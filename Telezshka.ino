#include <Array.h>
//#include <PID_v1.h>

const double koef = 94 / 75;

const double eps = 1.0;
Array<double, 6> xyz;


//double Setpoint, Input, Output;
//PID myPID(&Input, &Output, &Setpoint, 1, 4, 3, DIRECT);

const int potent1 = A1;
const int potent2 = A2;
const int potent3 = A3;

const int motorSpd1 = 2;
const int motorSpd2 = 3;
const int motorSpd3 = 4;

const int motorReverse1 = 22;
const int motorReverse2 = 23;
const int motorReverse3 = 24;


class wheel
{
    const int potentPin;
    const int motorSpdPin;
    const int motorReverseTurnPin;
    const double turnEps;
    const double koefPID;
    
    double angle;

    int stopTurn();
    /*TODO: Realize it!
    int stopMove();
    */
    int standAngle();
    int setTurnSpd(double spd);
    int rollClockwise();
    int rollCounterClock();

    double angleNow();

     
  public:
    wheel(int pot, int mS, int mR);
    int initialize();
    int moveToAngle();
    int setSpd(double spd);
    void setAngle(double angle_in);   
};

wheel::wheel(int pot, int mS, int mR)
:
  potentPin(pot),
  motorSpdPin(mS),
  motorReverseTurnPin(mR),
  turnEps(1.0),
  angle(165.0),
  koefPID(255. / 330.)
{
}

int wheel::initialize()
{
  pinMode(motorSpdPin, OUTPUT);
  pinMode(motorReverseTurnPin, OUTPUT);
  pinMode(potentPin, INPUT);
  
}
int wheel::stopTurn()
{
  analogWrite(motorSpdPin, 0);
}
void wheel::setAngle(double angle_in)
{
  angle = angle_in;
}
int wheel::moveToAngle()
{
  angle *= koef;
  if ((angle <= 330) && (angle >= 0))
  {
    standAngle();
  }  
}

int wheel::standAngle()
{
  double spd;
  double minDelta = 45;
  
  if (abs(angle - angleNow()) > turnEps)
  {
      double delta = angleNow() - angle;

      if (abs(delta) < minDelta)
      {
        delta > 0 ? spd = minDelta * koefPID : spd = -minDelta * koefPID;
      }
      else
      {
        spd = delta * koefPID;
      }

      setTurnSpd(spd);
      Serial.print(potentPin); 
      Serial.print(" Angle: ");
      Serial.print(angleNow());
      Serial.print(" Speed: ");
      Serial.println(spd);
  }
  else
  {
    stopTurn();
    Serial.println("Stopped!");
  }
  
}

double wheel::angleNow()
{
  if (potentPin != A3)
  {
    return analogRead(potentPin) * 330.0 / 1024.0;
  }
  else
  {
    //return (analogRead(potentPin) - 890) * 330. / 133.;
    return analogRead(potentPin) * 330.0 / 1024.0;
  }
}

int wheel::setTurnSpd(double spd)
{
  spd > 0 ? rollClockwise() : rollCounterClock();
  analogWrite(motorSpdPin, abs(spd));
}

int wheel::rollClockwise()
{
  digitalWrite(motorReverseTurnPin, HIGH);
}

int wheel::rollCounterClock()
{
  digitalWrite(motorReverseTurnPin, LOW);
}


wheel wheel1(potent1, motorSpd1, motorReverse1);
wheel wheel2(potent2, motorSpd2, motorReverse2);
wheel wheel3(potent3, motorSpd3, motorReverse3);

void setup()
{
  Serial.begin(9600);
  wheel1.initialize();
  wheel2.initialize();
  wheel3.initialize();
}

//180.0 0.0 0.0 0.0 0.0 0.0
//180.0 0.0 180.0 0.0 180.0 0.0
//205.0 0.0 125.0 0.0 65.0 0.0
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
    wheel1.setAngle(xyz.at(0));
    wheel2.setAngle(xyz.at(2));
    wheel3.setAngle(xyz.at(4));

  }
  moveTelejka();
}
 
