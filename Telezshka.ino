#include <Array.h>

#define mosfet 10
#define reverse 8

const double koef = 94 / 75;
int ptp1 = A1;
int ptp2 = A2;
int ptp3 = A3;
const double eps = 1.0;
Array<double, 6> xyz;

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

int moveHome()
{
  moveToAngle(165.0, eps, ptp1);
}

void setup()
{
  Serial.begin(9600);
  pinMode(mosfet, OUTPUT);
  pinMode(reverse, OUTPUT);

  moveHome();
}

void stopMove()
{
  Serial.println("Остановка");
  analogWrite(mosfet, 0);
}

double AngleNow1()
{
  return analogRead(ptp1) * 330.0 / 1024.0;
}

double AngleNow2()
{
  return analogRead(ptp2) * 330.0 / 1024.0;
}

double AngleNow3()
{
  return 3300.0 * ((5.0 / ((analogRead(ptp3) * 5.0) / 1024.0) ) - 1)  * 330.0 / 6300.0;
}

void rollClockwise()
{
  digitalWrite(reverse, HIGH);
}

void rollCounterClock()
{
  digitalWrite(reverse, LOW);
}

void standAngle(double degFinish, double epsilon, bool side, int numPTP)
{
  double eps1 = 10.0;
  int spd;
  (abs(degFinish - AngleNow(numPTP)) > eps1) ? spd = 255 : spd = 128;
  while (abs(degFinish - AngleNow(numPTP)) > epsilon)
  {
    side ? rollClockwise() : rollCounterClock();
    
    while (abs(degFinish - AngleNow(numPTP)) > eps1)
    {
      analogWrite(mosfet, spd);
      Serial.print("Angle = ");
      Serial.println(AngleNow(numPTP));
    }
    if (eps1 > 2)
    {
      eps1 /= 2;
    }
    if (spd > 16)
    {
      spd /= 2;
    }
    side = not side;
    delay(100);
    Serial.println("++++++++++++++++++++");
  }
  stopMove();
}
//180.0 0.0 0.0 0.0 0.0 0.0


double AngleNow(int numPTP)
{
  switch (numPTP)
  {
    case A1 : return AngleNow1();
    case A2 : return AngleNow2();
    case A3 : return AngleNow3();
  }
}


void moveToAngle(double degFinish, double epsilon, int numPTP)
{
  degFinish *= koef;
  if ((degFinish <= 330) && (degFinish >= 0))
  {
    if (degFinish < AngleNow(numPTP))
    {
      standAngle(degFinish, epsilon, true, numPTP);
    }
    else
    {
      standAngle(degFinish, epsilon, false, numPTP);
    }
  }
}

//void moveWheel(double sped)
//{
//  analogWrite(pinY, sped * 2.55);
//}


void moveTelejka(Array<double, 6> &xyz)
{
  moveToAngle(xyz[0], eps, ptp1);
  //moveWheel(xyz[1]);
  // moveToAngle(xyz[2], eps, PTP2);
  //moveWheel(xyz[3], PTP2);
  //moveToAngle(xyz[4], eps, PTP3);
  // moveWheel(xyz[5], PTP3);`

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
