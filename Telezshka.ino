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

void standSpeed()
{
  double delta = Speedwh - Speed_now;
  if (abs(delta) > eps)
  {
    setRollSpeed(delta);
  }
}

void moveWheel()
{
  if (Speedwh >= -715.0 && Speedwh <= 715.0)
  {
    standSpeed();
  }
}

void setRollSpeed(double Speed)
{
  Speed > 0 ? rollClockwise(whReverse) : rollCounterClock(whReverse);
  double new_Speed = pin_Speed + (abs((255 / max_Speed) * Speed)) / koefPIDSpeed;
  if (new_Speed >= 255)
  {
    analogWrite(whMosfet, 255);
    pin_Speed = 255;
  }
  else
  {
    analogWrite(whMosfet, new_Speed);
    pin_Speed = new_Speed;
  }
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

const int motorTurnSpeed1 = 2;
const int motorTurnSpeed2 = 3;
const int motorTurnSpeed3 = 4;

const int motorTurnReverse1 = 22;
const int motorTurnReverse2 = 23;
const int motorTurnReverse3 = 24;

const int opto1 = 8;
const int motorSpeedSpeed1 = 9;
const int motorSpeedReverse1 = 7;

class wheel
{
    const int potentPin;
    const int motorTurnSpeedPin;
    const int motorTurnReversePin;

    const int optoPin;
    const int motorRollSpeedPin;
    const int motorRollReversePin;
    
    const double turnEps;
    const double koefPID;

    double angle;
    double Speed;
    double speedNow;
    double epsSpeed;
    double sendSpeed;
    
    
    int stopTurn();
    /*TODO: Realize it!
      int stopMove();
    */
    int standAngle();
    int setTurnSpeed(double Speed);
    int setRollSpeed(double Speed);
    int rollClockwise();
    int rollCounterClock();
    
    int rollForward();
    int rollBackward();

    

    double angleNow();

    bool prevState;
    unsigned long prevTime;

  public:
    wheel(int pot, int mS, int mR, int opto, int mRSP, int mRR);
    int initialize();
    int moveToAngle();
    int setSpd(double SpeedIn);
    void setAngle(double angleIn);
    double getCurRollSpeed();
    int updateRoll();
    double curRollSpeed;
    void updateCurrentSpeed(double Speed_n);
};

wheel::wheel(int pot, int mS, int mR, int opto, int mRS, int mRR)
  :
  potentPin(pot),
  motorTurnSpeedPin(mS),
  motorTurnReversePin(mR),
  optoPin(opto),
  motorRollSpeedPin(mRS),
  motorRollReversePin(mRR),
  turnEps(1.0),
  angle(165.0),
  //koefPID(255. / 330.)
  koefPID(1.4),
  prevTime(micros()),
  sendSpeed(0),
  speedNow(0),
  curRollSpeed(0)
{
  pinMode(motorTurnSpeedPin, OUTPUT);
  pinMode(motorTurnReversePin, OUTPUT);
  pinMode(potentPin, INPUT);
  pinMode(optoPin, INPUT);
  pinMode(motorRollSpeedPin, OUTPUT);
  pinMode(motorRollReversePin, OUTPUT);
  
  prevState = digitalRead(optoPin);
}

void wheel::updateCurrentSpeed(double Speed_n)
{
  speedNow = Speed_n;
}

int wheel::setRollSpeed(double SpeedVal)
{
  SpeedVal > 0 ? rollForward() : rollBackward();
  double Speed = abs(SpeedVal);
  if (Speed > 255)
  {
    analogWrite(motorRollSpeedPin, 255);
    sendSpeed = 255;
  }
  else
  {
    analogWrite(motorRollSpeedPin, Speed);
    sendSpeed = Speed;
  }
}

int wheel::rollBackward()
{
  digitalWrite(motorRollReversePin, HIGH);
}

int wheel::rollForward()
{
  digitalWrite(motorRollReversePin, LOW);
}

double wheel::getCurRollSpeed()
{
  bool curState = digitalRead(optoPin);
  if (curState != prevState)
  {
    unsigned long stepTime = micros() - prevTime;
    prevState = curState;
    prevTime = micros();
    
    curState == true ? stepTime *= 1.382239382 : stepTime *= 0.7306122449;
    static_cast<double>(stepTime);
    
    speedNow = 5. / (stepTime / 1e6);  // 5 mm / (time in microseconds / 1 000 000);
    return speedNow;
  }
  else
  {
    return 0;
  }
}

int wheel::setSpd(double speedIn)
{
  Speed = speedIn;
}

int wheel::updateRoll()
{
  double delta = (Speed - speedNow);
  if (abs(delta) > epsSpeed)
  {
    setRollSpeed(delta);
  }
}

wheel wheel1(potent1, motorTurnSpeed1, motorTurnReverse1, opto1, motorSpeedSpeed1, motorSpeedReverse1);
//wheel wheel2(potent2, motorTurnSpeed2, motorTurnReverse2);
//wheel wheel3(potent3, motorTurnSpeed3, motorTurnReverse3);

void setup()
{
  Serial.begin(9600);
  //wheel1.initialize();
  //wheel2.initialize();
  //wheel3.initialize();
  Serial.println("Started!");
}

//180.0 0.0 0.0 0.0 0.0 0.0
//180.0 0.0 180.0 0.0 180.0 0.0
//205.0 0.0 125.0 0.0 65.0 0.0
void moveTelejka()
{
  wheel1.moveToAngle();
  //wheel2.moveToAngle();
  //wheel3.moveToAngle();

}

// 0.0 10.0 0.0 0.0 0.0 0.0
void loop()
{
  if (Serial.available() > 0)
  {
    for (int i = 0; i < 6; ++i)
    {
      xyz.at(i) = Serial.parseFloat();
      Serial.println(xyz.at(i));
    }
    wheel1.setSpd(xyz.at(1));
  }
    
  double tmp = wheel1.getCurRollSpeed();
  if (tmp > 0)
  {
    Serial.println(tmp);
  }
  //moveTelejka();
}

