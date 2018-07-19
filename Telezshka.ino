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
const int motorReverse1 = 22;

const int motorSpd2 = 3;
const int motorReverse2 = 23;

class wheel
{
    /*const int potentPin;
    const int motorSpdPin;
    const int motorReverseTurnPin;
    const double turnEps;
    const double koefPID;*/

    int potentPin;
    int motorSpdPin;
    int motorReverseTurnPin;
    double turnEps;
    double koefPID;


    int stopTurn();
    /*TODO: Realize it!
    int stopMove();
    */
    int standAngle(double angle);
    int setTurnSpd(double spd);
    int rollClockwise();
    int rollCounterClock();

    double angleNow();

    int moveHome();
    
  public:
    wheel(int pot, int mS, int mR);
    int initialize();
    int moveToAngle(double angle);
    int setSpd(double spd);
    
};

wheel::wheel(int pot, int mS, int mR)
:
  potentPin(pot),
  motorSpdPin(mS),
  motorReverseTurnPin(mR),
  turnEps(1.0),
  koefPID(255. / 330.)
{
}

int wheel::initialize()
{
  pinMode(motorSpdPin, OUTPUT);
  pinMode(motorReverseTurnPin, OUTPUT);
  pinMode(potentPin, INPUT);
  
  moveHome();
}

int wheel::moveHome()
{
  moveToAngle(165.0);
}
int wheel::stopTurn()
{
  analogWrite(motorSpdPin, 0);
}

int wheel::moveToAngle(double angle)
{
  angle *= koef;
  if ((angle <= 330) && (angle >= 0))
  {
    standAngle(angle);
  }  
}

int wheel::standAngle(double angle)
{
  double spd;
  double minDelta = 45;
  
  do
  {
    do
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
      Serial.print("Angle: ");
      Serial.print(angleNow());
      Serial.print(" Speed: ");
      Serial.println(spd);
    }
    while (abs(angle - angleNow()) > turnEps);
    delay(50);
  }
  while (abs(angle - angleNow()) > turnEps);
  stopTurn();
  Serial.println("Stopped!");

  
}

double wheel::angleNow()
{
  return analogRead(potentPin) * 330.0 / 1024.0;
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

void setup()
{
  Serial.begin(9600);
  //Serial.println("1st started");
  wheel1.initialize();
  //Serial.println("1st stopped\n2nd started");
  wheel2.initialize();
  //Serial.println("2nd stopped");
}

//180.0 0.0 0.0 0.0 0.0 0.0

void moveTelejka(Array<double, 6> &xyz)
{
  wheel1.moveToAngle(xyz[0]);
  wheel2.moveToAngle(xyz[0]);

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
    moveTelejka(xyz);
  }
}
