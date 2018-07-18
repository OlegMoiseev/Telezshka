#include <Array.h>
//#include <PID_v1.h>

#define mosfet 10
#define reverse 8

const double koef = 94 / 75;
int ptp1 = A1;
int ptp2 = A2;
int ptp3 = A3;
const double eps = 1.0;
Array<double, 6> xyz;


double Setpoint, Input, Output;
 
//PID myPID(&Input, &Output, &Setpoint, 1, 4, 3, DIRECT);


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
  moveToAngle(165.0, ptp1);
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



void setRotSpeed(double spd)
{
  spd > 0 ? rollClockwise() : rollCounterClock();
  Serial.print("Send: ");
  Serial.print(spd);
  Serial.print(" ");

  analogWrite(mosfet, abs(spd));  
}

void standAngle(double degFinish, int numPTP)
{
  double epsilon = 1.0;
  double spd;
  double k = 255. / 330.;
  //double k = 1;
  double minDelta = 45;
  double prevSpd = 0;
  do
  {
    do
    {
      double nowA = AngleNow(numPTP);
      
      double delta = nowA - degFinish;
  
      if (abs(delta) < minDelta)
      {
        delta > 0 ? spd = minDelta * k : spd = -minDelta * k;
  
      }
      else
      {
        spd = delta * k;
      }

      setRotSpeed(spd);

      Serial.print("Angle now: ");
      Serial.print(nowA);
      Serial.print(" Speed: ");
      Serial.println(spd);
    }
    while (abs(degFinish - AngleNow(numPTP)) > epsilon);
    delay(10);
  }
  while (abs(degFinish - AngleNow(numPTP)) > epsilon);
  stopMove();

  Serial.println(abs(degFinish - AngleNow(numPTP)) > epsilon);
  Serial.println("EXITED");
  Serial.println(degFinish - AngleNow(numPTP));
  
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


void moveToAngle(double degFinish, int numPTP)
{
  degFinish *= koef;
  if ((degFinish <= 330) && (degFinish >= 0))
  {
    standAngle(degFinish, numPTP);
  }
}


void moveTelejka(Array<double, 6> &xyz)
{
  moveToAngle(xyz[0], ptp1);
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
