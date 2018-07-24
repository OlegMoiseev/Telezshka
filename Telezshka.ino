#include <Array.h>

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
    int standAngle();
    int setTurnSpd(double spd);
    int rollClockwise();
    int rollCounterClock();

    double getCurrentAngle();


  public:
    wheel(int pot, int mS, int mR);
    int moveToAngle();
    void setAngle(double angle_in);
};

wheel::wheel(int pot, int mS, int mR)
  :
  potentPin(pot),
  motorTurnSpdPin(mS),
  motorTurnReversePin(mR),
  turnEps(1.0),
  angle(175.0),
  //turnKoefPID(255. / 330.)
  turnKoefPID(1.4),
  gearsKoef(94. / 75.)
{
  pinMode(motorTurnSpdPin, OUTPUT);
  pinMode(motorTurnReversePin, OUTPUT);
  pinMode(potentPin, INPUT);
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
  if ((angle <= 330) && (angle >= 0))
  {
    standAngle();
  }
}

int wheel::standAngle()
{
  double spd;
  double minDelta = 30;

  if (abs(angle - getCurrentAngle()) > turnEps)
  {
    double delta = getCurrentAngle() - angle;

    if (abs(delta) < minDelta)
    {
      delta > 0 ? spd = minDelta * turnKoefPID : spd = -minDelta * turnKoefPID;
    }
    else
    {
      spd = delta * turnKoefPID;
    }

    setTurnSpd(spd);
    Serial.print(potentPin);
    Serial.print(" Angle: ");
    Serial.print(getCurrentAngle());
    Serial.print(" Speed: ");
    Serial.println(spd);
  }
  else
  {
    stopTurn();
    Serial.println("Stopped!");
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

wheel wheel1(potent1, motorTurnSpd1, motorTurnReverse1);
wheel wheel2(potent2, motorTurnSpd2, motorTurnReverse2);
wheel wheel3(potent3, motorTurnSpd3, motorTurnReverse3);

void setup()
{
  Serial.begin(9600);
}

//180.0 0.0 0.0 0.0 0.0 0.0
//180.0 0.0 180.0 0.0 180.0 0.0
//30.0 0.0 -30.0 0.0 -90.0 0.0

void moveTelejka()
{
  wheel1.moveToAngle();
  wheel2.moveToAngle();
  wheel3.moveToAngle();
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
    wheel2.setAngle(xyz.at(2));
    wheel3.setAngle(xyz.at(4));

  }
  moveTelejka();
}

