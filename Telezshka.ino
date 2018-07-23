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
  double delta = spdwh - spd_now;
  if (abs(delta) > eps)
  {
    setRollSpd(delta);
  }
}

void moveWheel()
{
  if (spdwh >= -715.0 && spdwh <= 715.0)
  {
    standSpeed();
  }
}

void setRollSpd(double spd)
{
  spd > 0 ? rollClockwise(whReverse) : rollCounterClock(whReverse);
  double new_spd = pin_spd + (abs((255 / max_spd) * spd)) / koefPIDspd;
  if (new_spd >= 255)
  {
    analogWrite(whMosfet, 255);
    pin_spd = 255;
  }
  else
  {
    analogWrite(whMosfet, new_spd);
    pin_spd = new_spd;
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

const int motorTurnSpd1 = 2;
const int motorTurnSpd2 = 3;
const int motorTurnSpd3 = 4;

const int motorTurnReverse1 = 22;
const int motorTurnReverse2 = 23;
const int motorTurnReverse3 = 24;

const int opto1 = 8;
const int motorSpeedSpd1 = 9;
const int motorSpeedReverse1 = 7;

class wheel
{
    const int potentPin;
    const int motorTurnSpdPin;
    const int motorTurnReversePin;

    const int optoPin;
    const int motorRollSpdPin;
    const int motorRollReversePin;
    
    const double turnEps;
    const double koefPID;


    double angle;
    double spd;
    double speedNow;
    double epsSpd;
    double sendSpd;
    
    
    int stopTurn();
    /*TODO: Realize it!
      int stopMove();
    */
    int standAngle();
    int setTurnSpd(double spd);
    int setRollSpd(double spd);
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
    int setSpd(double spdIn);
    void setAngle(double angleIn);
    double getCurRollSpd();
    int updateRoll();
    double curRollSpd;
    void updateCurrentSpd(double spd_n);
};

wheel::wheel(int pot, int mS, int mR, int opto, int mRS, int mRR)
  :
  potentPin(pot),
  motorTurnSpdPin(mS),
  motorTurnReversePin(mR),
  optoPin(opto),
  motorRollSpdPin(mRS),
  motorRollReversePin(mRR),
  turnEps(1.0),
  angle(165.0),
  //koefPID(255. / 330.)
  koefPID(1.4),
  prevState(false),
  prevTime(millis()),
  sendSpd(0),
  epsSpd(50),
  spd(0),
  speedNow(0),
  curRollSpd(0)
{
}

int wheel::initialize()
{
  pinMode(motorTurnSpdPin, OUTPUT);
  pinMode(motorTurnReversePin, OUTPUT);
  pinMode(potentPin, INPUT);
  pinMode(optoPin, INPUT);
  pinMode(motorRollSpdPin, OUTPUT);
  pinMode(motorRollReversePin, OUTPUT);
}
void wheel::updateCurrentSpd(double spd_n)
{
  speedNow = spd_n;
}

int wheel::setRollSpd(double spdVal)
{
  spdVal > 0 ? rollForward() : rollBackward();
  double spd = abs(spdVal);
  if (spd > 255)
  {
    analogWrite(motorRollSpdPin, 255);
    sendSpd = 255;
  }
  else
  {
    analogWrite(motorRollSpdPin, spd);
    sendSpd = spd;
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

double wheel::getCurRollSpd()
{
  double oneTooth = 360. / 128.;
  {
    return 0;
  }
}

int wheel::setSpd(double spdIn)
{
  spd = spdIn;
}

int wheel::updateRoll()
{
  double delta = (spd - speedNow);
  if (abs(delta) > epsSpd)
  {
    setRollSpd(delta);
  }
}


wheel wheel1(potent1, motorTurnSpd1, motorTurnReverse1, opto1, motorSpeedSpd1, motorSpeedReverse1);
//wheel wheel2(potent2, motorTurnSpd2, motorTurnReverse2);
//wheel wheel3(potent3, motorTurnSpd3, motorTurnReverse3);

void setup()
{
  Serial.begin(9600);
  wheel1.initialize();
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
      //Serial.println(xyz.at(i));
    }
    wheel1.setSpd(xyz.at(1));
  }  
  double tmp = wheel1.getCurRollSpd();
  if (tmp > 0)
  {
    Serial.println(tmp);
  }
 //wheel1.setAngle(xyz.at(0));
  // wheel1.setSpd(xyz.at(1));
   /*Serial.print("Speed now :  ");
   Serial.print(wheel1.get_spd_now());
   Serial.print(" Speed will : ");
   Serial.println(xyz.at(1));*/
   //wheel2.setAngle(xyz.at(2));
   //wheel3.setAngle(xyz.at(4));
   //wheel1.updateRoll();
  //moveTelejka();
}

