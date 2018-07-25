#include <Array.h>

double speed4wheel;

Array<double, 6> xyz;

const int potent1 = A1;
const int potent2 = A2;
const int potent3 = A3;

const int motorTurnSpd1 = 2;
const int motorTurnSpd2 = 3;
const int motorTurnSpd3 = 4;

const int motorTurnReverse1 = 22;
const int motorTurnReverse2 = 23;
const int motorTurnReverse3 = 24;

const int opto1 = 29;
const int opto2 = 28;
const int opto3 = 30;

const int motorRollSpd1 = 5;
const int motorRollSpd2 = 6;
const int motorRollSpd3 = 7;

const int motorRollReverse1 = 25;
const int motorRollReverse2 = 26;
const int motorRollReverse3 = 27;

class wheel
{
    const int potentPin;
    const int motorTurnSpdPin;
    const int motorTurnReversePin;
    const double turnEps;
    const double turnKoefPID;
    const double gearsKoef;

    double angle;

    int stopTurn();
    int updateAngle();
    int setTurnSpd(double spd);
    int rollClockwise();
    int rollCounterClock();
    double getCurrentAngle();

    const int optoPin;
    const int motorRollSpdPin;
    const int motorRollReversePin;

    double sendSpd;
    double desiredSpd;
    double currentSpd;

    bool prevState;
    unsigned long prevTime;

    int rollForward();
    int rollBackward();

    void updateCurRollSpd();

  public:
    wheel(int pot, int mS, int mR, int opto, int mRSP, int mRR);
    int setRollSpd(double spdIn);
    void updateRollSpd();
    void stopMove();

    int moveToAngle();
    void setAngle(double angle_in);
};

wheel::wheel(int pot, int mS, int mR, int opto, int mRSP, int mRR)
  :
  potentPin(pot),
  motorTurnSpdPin(mS),
  motorTurnReversePin(mR),
  turnEps(1.0),
  angle(175.0),
  turnKoefPID(1.4),
  gearsKoef(94. / 75.),

  optoPin(opto),
  motorRollSpdPin(mRSP),
  motorRollReversePin(mRR),
  prevTime(micros()),
  sendSpd(0),
  currentSpd(0),
  desiredSpd(0)
{
  pinMode(motorTurnSpdPin, OUTPUT);
  pinMode(motorTurnReversePin, OUTPUT);
  pinMode(potentPin, INPUT);

  pinMode(optoPin, INPUT);
  pinMode(motorRollSpdPin, OUTPUT);
  pinMode(motorRollReversePin, OUTPUT);

  prevState = digitalRead(optoPin);
}

int wheel::rollBackward()
{
  digitalWrite(motorRollReversePin, HIGH);
}

int wheel::rollForward()
{
  digitalWrite(motorRollReversePin, LOW);
}

void wheel::updateCurRollSpd()
{
  bool curState = digitalRead(optoPin);
  if (curState != prevState)
  {
    unsigned long stepTime = micros() - prevTime;

    prevState = curState;
    prevTime = micros();
    curState == true ? stepTime *= 1.382239382 : stepTime *= 0.7306122449;
    currentSpd = 5. / (static_cast<double>(stepTime) / 1e6);  // 5 mm / (time in microseconds / 1 000 000);
  }
}

int wheel::setRollSpd(double spdIn)
{
  desiredSpd = spdIn;
}

void wheel::stopMove()
{
  analogWrite(motorRollSpdPin, 0);
}

void wheel::updateRollSpd()
{
  if (abs(desiredSpd) < 1.0)
  {
    stopMove();
  }
  else
  {
    updateCurRollSpd();
    desiredSpd > 0 ? rollForward() : rollBackward();
    double delta = abs(desiredSpd) - currentSpd;
    if (abs(delta) > 10.0)
    {
      sendSpd += (255.0 / 200.0) * delta / 500.0;  // 200 - maxSpeed 2.0 - koef PID
      if (sendSpd > 255)
      {
        sendSpd = 255;
      }
      if (sendSpd < 0)
      {
        sendSpd = 50;
      }

// ********************************************
// It's big (very, very big!) crutch while we haven't last gear to measure wheel's speed!
// Now we synchronize 1st and 3rd wheels
// ********************************************
      if (motorRollSpdPin == motorRollSpd1)
      {
        speed4wheel = sendSpd;
      }
      if (motorRollSpdPin == motorRollSpd3)
      {
        sendSpd = speed4wheel;
      }
// ********************************************
      analogWrite(motorRollSpdPin, sendSpd);
    }
  }
}
int wheel::stopTurn()
{
  analogWrite(motorTurnSpdPin, 0);
}

void wheel::setAngle(double angle_in)
{
  if (abs(angle_in) > 120)
  {
    if (angle_in > 120)
    {
      angle_in = 120;
    }
    if (angle_in < -120)
    {
      angle_in = -120;
    }

  }
  angle = (angle_in + 140) * gearsKoef;
}

int wheel::moveToAngle()
{
  double curA = getCurrentAngle();
  if (curA < 10 or curA > 320)
  {
    stopTurn();
  }
  if ((angle <= 330) && (angle >= 0))
  {
    updateAngle();
  }
}

int wheel::updateAngle()
{
  double spd;
  double minDelta = 30;
  double curAng = getCurrentAngle();
  if (abs(angle - curAng) > turnEps)
  {
    double delta = curAng - angle;

    if (abs(delta) < minDelta)
    {
      delta > 0 ? spd = minDelta * turnKoefPID : spd = -minDelta * turnKoefPID;
    }
    else
    {
      spd = delta * turnKoefPID;
    }
    setTurnSpd(spd);
  }
  else
  {
    stopTurn();
    //Serial.println("Stopped!");
  }
}

double wheel::getCurrentAngle()
{
  return analogRead(potentPin) * 330.0 / 1024.0;
}

int wheel::setTurnSpd(double spd)
{
  spd > 0 ? rollClockwise() : rollCounterClock();
  analogWrite(motorTurnSpdPin, abs(spd));
}

int wheel::rollClockwise()
{
  digitalWrite(motorTurnReversePin, HIGH);
}

int wheel::rollCounterClock()
{
  digitalWrite(motorTurnReversePin, LOW);
}

// 180.0 0.0 0.0 0.0 0.0 0.0
// 180.0 0.0 180.0 0.0 180.0 0.0
// 30.0 0.0 -30.0 0.0 -90.0 0.0
// 0.0 0.0 0.0 0.0 0.0 0.0
// 0.0 20.0 0.0 20.0 0.0 20.0

wheel wheel1(potent1, motorTurnSpd1, motorTurnReverse1, opto1, motorRollSpd1, motorRollReverse1);
wheel wheel2(potent2, motorTurnSpd2, motorTurnReverse2, opto2, motorRollSpd2, motorRollReverse2);
wheel wheel3(potent3, motorTurnSpd3, motorTurnReverse3, opto3, motorRollSpd3, motorRollReverse3);

void moveTelejka()
{
  wheel1.moveToAngle();
  wheel1.updateRollSpd();

  wheel2.moveToAngle();
  wheel2.updateRollSpd();

  wheel3.moveToAngle();
  wheel3.updateRollSpd();
}

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(0);
  Serial.println("Started!");
}

void loop()
{
  if (Serial.available() > 0)
  {
    for (int i = 0; i < 6; ++i)
    {
      xyz.at(i) = Serial.parseFloat();
      Serial.println(xyz.at(i));
    }
    Serial.flush();

    wheel1.setAngle(xyz.at(0));
    wheel1.setRollSpd(xyz.at(1));

    wheel2.setAngle(xyz.at(2));
    wheel2.setRollSpd(xyz.at(3));

    wheel3.setAngle(xyz.at(4));
    wheel3.setRollSpd(xyz.at(5));
  }
  moveTelejka();
}

