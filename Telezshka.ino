/*double check(int pinTh)
{
  if (digitalRead(pinTh) && f == false)
    {
    digitalWrite(13, HIGH);
    t1 = millis();
    f = true;
    if (t2 != 0 && t2 < t1)
    {
      Serial.print((double)(10.0/((t1 - t2)/1000.0)));
      Serial.println("  ");
      return (double)(10.0/((t1 - t2)/1000.0));
    }
    }
    if (!digitalRead(pinTh) && f == true)
    {
    digitalWrite(13, LOW);
    t2 = millis();
    f = false;
    }
}

void standspd()
{
  double delta = spd - speedNow;
  if (abs(delta > eps)
  {
    double new_spd = pin_spd + (abs((255 / max_spd) * delta)) / koefPIDspd
    if (new_spd >= 255)
    {
      pin_spd = 255;
    }
    else
    {
      pin_spd = new_spd;
    }
    setRollspd(pin_spd);
  }
}

void moveWheel()
{
  if (spdwh >= -715.0 && spdwh <= 715.0)
  {
    standspd();
  }
}

void setRollspd(double spd)
{
  spd > 0 ? rollClockwise(whReverse) : rollCounterClock(whReverse);
  analogWrite(motorRollspdPin, spd);
}*/



#include <Array.h>
#include <PID_v1.h>

const double koef = 94 / 75;

const double eps = 1.0;
Array<double, 6> xyz;


//double Setpoint, Input, Output;
//PID myPID(&Input, &Output, &Setpoint, 1, 4, 3, DIRECT);

const int potent1 = A1;
const int potent2 = A2;
const int potent3 = A3;

const int motorTurnspd1 = 2;
const int motorTurnspd2 = 3;
const int motorTurnspd3 = 4;

const int motorTurnReverse1 = 22;
const int motorTurnReverse2 = 23;
const int motorTurnReverse3 = 24;

const int opto1 = 8;
const int motorSpdSpd1 = 9;
const int motorSpdReverse1 = 7;

class wheel
{
    const int potentPin;
    const int motorTurnspdPin;
    const int motorTurnReversePin;

    const int optoPin;
    const int motorRollspdPin;
    const int motorRollReversePin;
    
    const double turnEps;
    const double koefPID;
 
    
    int stopTurn();
    /*TODO: Realize it!
      int stopMove();
    */
    int standAngle();
    int setTurnspd(double spd);

    int rollClockwise();
    int rollCounterClock();
    
    int rollForward();
    int rollBackward();
    
    double angleNow();

    bool prevState;
    unsigned long prevTime;

  public:
    double sendSpd;
    double desiredSpd;
    double currentSpd;
    
    wheel(int pot, int mS, int mR, int opto, int mRSP, int mRR);
    int setSpd(double spdIn);
    void updateCurRollSpd();
    int setRollSpd();
};

wheel::wheel(int pot, int mS, int mR, int opto, int mRS, int mRR)
  :
  potentPin(pot),
  motorTurnspdPin(mS),
  motorTurnReversePin(mR),
  optoPin(opto),
  motorRollspdPin(mRS),
  motorRollReversePin(mRR),
  turnEps(1.0),
  //koefPID(255. / 330.)
  koefPID(1.4),
  prevTime(micros()),
  sendSpd(0),
  currentSpd(0),
  desiredSpd(0)
{
  pinMode(motorTurnspdPin, OUTPUT);
  pinMode(motorTurnReversePin, OUTPUT);
  pinMode(potentPin, INPUT);
  pinMode(optoPin, INPUT);
  pinMode(motorRollspdPin, OUTPUT);
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
    static_cast<double>(stepTime);
    currentSpd = 5. / (stepTime / 1e6);  // 5 mm / (time in microseconds / 1 000 000);
  }
}

int wheel::setSpd(double spdIn)
{
  desiredSpd = spdIn;
}

int wheel::setRollSpd()
{
  updateCurRollSpd();
  desiredSpd > 0 ? rollForward() : rollBackward();
  double delta = abs(desiredSpd) - currentSpd;
  if (abs(delta) > 10.0)
  {
    sendSpd += (255.0 / 200.0) * delta / 100.0;  // 200 - maxSpeed 2.0 - koef PID 
    if (sendSpd > 255)
    {
      sendSpd = 255;
    }
    if (sendSpd < 0)
    {
      sendSpd = 50;
    }
    analogWrite(motorSpdSpd1, sendSpd);
  }
  Serial.print("Current speed:\t");
  Serial.print(currentSpd);
  Serial.print("\tDesired speed:\t");
  Serial.print(desiredSpd);
  Serial.print("\tDelta:\t");
  Serial.print(delta);
  Serial.print("\tSend speed:\t");
  Serial.println(sendSpd);
  
  analogWrite(motorRollspdPin, sendSpd);
}


wheel wheel1(potent1, motorTurnspd1, motorTurnReverse1, opto1, motorSpdSpd1, motorSpdReverse1);

void setup()
{
  Serial.begin(115200);
  Serial.println("Started!");
}

//180.0 0.0 0.0 0.0 0.0 0.0
//180.0 0.0 180.0 0.0 180.0 0.0
//205.0 0.0 125.0 0.0 65.0 0.0



// 0.0 50.0 0.0 0.0 0.0 0.0
void loop()
{
  if (Serial.available() > 0)
  {
    for (int i = 0; i < 6; ++i)
    {
      xyz.at(i) = Serial.parseFloat();
      //Serial.println(xyz.at(i));
    }
    wheel1.desiredSpd = xyz.at(1);
  }
  
  wheel1.setRollSpd();
}

