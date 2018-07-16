#include <Array.h>
#include "U8glib.h"


int pinY = 10;
int pinSensor = 5;
int PTP1 = A1;
int PTP2 = A2;
int PTP3 = A3;
double eps = 1.0, a1 = 0.0, a2 = 0.0, a3 = 0.0;
Array<double, 6> xyz;

//display init
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_FAST);

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
    //void draw(void);
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
  moveToAngle(165.0, eps, PTP1);
}

void draw(void)
{
  u8g.setFont(u8g_font_5x8);
  u8g.setPrintPos(42, 10); u8g.print("|");
  u8g.setPrintPos(42, 20); u8g.print("|");
  u8g.setPrintPos(42, 30); u8g.print("|");
  u8g.setPrintPos(42, 40); u8g.print("|");
  u8g.setPrintPos(42, 50); u8g.print("|");
  u8g.setPrintPos(42, 60); u8g.print("|");
  u8g.setPrintPos(84, 10); u8g.print("|");
  u8g.setPrintPos(84, 20); u8g.print("|");
  u8g.setPrintPos(84, 30); u8g.print("|");
  u8g.setPrintPos(84, 40); u8g.print("|");
  u8g.setPrintPos(84, 50); u8g.print("|");
  u8g.setPrintPos(84, 60); u8g.print("|");
  //speed
  u8g.setPrintPos(10, 10); u8g.print("U1");
  u8g.setPrintPos(10, 20); u8g.print(xyz[1]);
  u8g.setPrintPos(10, 30); u8g.print("0");

  u8g.setPrintPos(50, 10); u8g.print("U2");
  u8g.setPrintPos(50, 20); u8g.print(xyz[3]);
  u8g.setPrintPos(50, 30); u8g.print("0");

  u8g.setPrintPos(90, 10); u8g.print("U3");
  u8g.setPrintPos(90, 20); u8g.print(xyz[5]);
  u8g.setPrintPos(90, 30); u8g.print("0");
  //degrees
  u8g.setPrintPos(10, 40); u8g.print("G1");
  u8g.setPrintPos(10, 50); u8g.print(xyz[0]);
  u8g.setPrintPos(10, 60); u8g.print(AngleNow1());

  u8g.setPrintPos(50, 40); u8g.print("G2");
  u8g.setPrintPos(50, 50); u8g.print(xyz[2]);
  u8g.setPrintPos(50, 60); u8g.print(AngleNow2());

  u8g.setPrintPos(90, 40); u8g.print("G3");
  u8g.setPrintPos(90, 50); u8g.print(xyz[4]);
  u8g.setPrintPos(90, 60); u8g.print(AngleNow3());
}

void setup() {
  //display setup
  if (u8g.getMode() == U8G_MODE_R3G3B2)
  {
    u8g.setColorIndex(255);     // white
  }
  else if (u8g.getMode() == U8G_MODE_GRAY2BIT)
  {
    u8g.setColorIndex(3);         // max intensity
  }
  else if (u8g.getMode() == U8G_MODE_BW)
  {
    u8g.setColorIndex(1);         // pixel on
  }
  else if (u8g.getMode() == U8G_MODE_HICOLOR)
  {
    u8g.setHiColorByRGB(255,255,255);
  }
  //--
  
  Serial.begin(9600);
  pinMode(pinY, OUTPUT);
  pinMode(pinSensor, OUTPUT);
  moveHome();
}

void stopMove() {
  Serial.println("Остановка");
  analogWrite(pinY, 0);
}

double AngleNow1()
{
  return analogRead(PTP1) * 330.0 / 1024.0;
}

double AngleNow2()
{
  return analogRead(PTP2) * 330.0 / 1024.0;
}

double AngleNow3()
{
  return 3300.0 * ((5.0 / ((analogRead(PTP3) * 5.0) / 1024.0) ) - 1)  * 330.0 / 6300.0;
}

void rollClockwise()
{
  digitalWrite(pinSensor, HIGH);
}

void rollCounterClock()
{
  digitalWrite(pinSensor, LOW);
}

void standAngle(double degFinish, double epsilon, bool side, int numPTP)
{//Oleg
  int spd = 255;
  while (abs(degFinish - AngleNow(numPTP)) > epsilon){
      
    side ? rollClockwise() : rollCounterClock();
  
    while (abs(degFinish - AngleNow(numPTP)) > epsilon)
    {
      analogWrite(pinY, 40);
      Serial.print("num = ");
      Serial.print(numPTP);
      Serial.print(" angle = ");
      Serial.println(AngleNow(numPTP));

      
    }
    delay(100);
    side = not side;
    spd = spd / 2;
    Serial.println("++++++++++++++++++++");
  }
  stopMove();
}

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

void moveWheel(double sped)
{
  analogWrite(pinY, sped * 2.55);
}


void moveTelejka(Array<double, 6> &xyz)
{
  moveToAngle(xyz[0], eps, PTP1);
  //moveWheel(xyz[1]);
  // moveToAngle(xyz[2], eps, PTP2);
  //moveWheel(xyz[3], PTP2);
  //moveToAngle(xyz[4], eps, PTP3);
  // moveWheel(xyz[5], PTP3);`

}

void loop()
{ 
//  u8g.firstPage();
//  do
//  {
//    draw();
//  } while (u8g.nextPage());

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
