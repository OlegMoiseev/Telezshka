#include <Array.h>

//#define mosfet 10
#define whMosfet 11
//#define reverse 8
#define whReverse 7

const double koef = 94 / 75;
const double onetik = 180/32;
int pinTh = 8;
int ptp1 = A1;
int ptp2 = A2;
int ptp3 = A3;
double spdwh = 0.0;
double spd_now = 0.0;
double pin_spd = 0.0;
double max_spd = 715.0;
double koefPIDspd = 2.0;
int s;
int i = 0;
double eps = 10.0;
double deg;
bool f = false;
//const double eps = 1.0;
Array<double, 6> xyz;
double Setpoint, Input, Output;
double t1 = 0.0, t2 = 0.0;

class Data
{
  private:
    double deg_1;
    double deg_2;
    double deg_3;
    double spd_1;
    double spd_2;
    double spd_3;
    int getData();

  public:
    Data();
    int initSerial();
    int moveHome();
    int movePoint();
};

Data::Data()
  : deg_1(165), deg_2(165), deg_3(165),
    spd_1(0), spd_2(0), spd_3(0)
{
}

int Data::initSerial()
{
  Serial.begin(9600);
}

void setup()
{
  Serial.begin(9600);
//  pinMode(mosfet, OUTPUT);
//  pinMode(reverse, OUTPUT);
  pinMode(whReverse, OUTPUT);
  pinMode(whMosfet, OUTPUT);
  pinMode(pinTh, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
}

double check(int pinTh)
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
  {`
    digitalWrite(13, LOW);
    t2 = millis();
    f = false;
  }
}

void rollClockwise(int rev)
{
  digitalWrite(rev, HIGH);
}

void rollCounterClock(int rev)
{
  digitalWrite(rev, LOW);
}

//180.0 0.0 0.0 0.0 0.0 0.0

void moveTelejka(Array<double, 6> &xyz)
{
  //moveToAngle(xyz[0], ptp1);
  spdwh = xyz.at(1);
  moveWheel();
  // moveToAngle(xyz[2], ptp2);
  //moveWheel(xyz[3]);
  //moveToAngle(xyz[4], ptp3);
  // moveWheel(xyz[5]);`

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
}

void loop()
{ 
  if (Serial.available() > 0)
  {
      for (int i = 0; i < 6; ++i)
      {
        xyz.at(i) = Serial.parseFloat();
        //Serial.println(xyz.at(i));
      }
  }
  //Serial.println(check(pinTh));
  //spd_now = (double) check(pinTh);
  //Serial.print(spd_now);
  moveTelejka(xyz);
}
