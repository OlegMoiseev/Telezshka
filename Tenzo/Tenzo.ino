#include "HX711.h"
#include <Array.h>

#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

const int CLK2 = 2; //2 * i + 2 == 0 + 2
const int DOUT2 = 3; //2 * i + 3 == 0 + 3

const int CLK3 = 4;  //1 + 3 == 2 * i + 2
const int DOUT3 = 5;  //1 + 4 == 2 * i + 3

const int CLK4 = 6;  //2 + 4
const int DOUT4 = 7;  //2 + 5

const int CLK5 = 8;  //3 + 5 == 2 * i + 2
const int DOUT5 = 9;  //3 + 6 == 2 * i + 3

double epsForce = 3.0;
bool check = true;
const int emergencyStop = 0;
Array<HX711, 9> tenzo;

const int numSensors = 7;

void setup()
{
//  Serial.begin(9600);
//  Serial.setTimeout(100);

  for (int i = 0; i < numSensors; ++i)
  {
    if (i > 4)
    {
      tenzo[i] = HX711(2 * (i + 1) + 2, 2 * (i + 1) + 3);
    }
    else
    {
      tenzo[i] = HX711(2 * i + 2, 2 * i + 3);
    }
    tenzo[i].set_scale(calibration_factor);
    tenzo[i].tare();
  }
  pinMode(emergencyStop, OUTPUT);
}

void tenzo_dat()
{
  bool allStop = false;
  double force;
  String out = "";

  for (int i = 0; i < numSensors; ++i)
  {
    force = tenzo[i].get_units();
    out += String(force) + '\t';
    
    if (abs(force) > epsForce)
    {
      //Serial.print("Collision!\t");
      digitalWrite(emergencyStop, LOW);
      allStop = true;
    }
  }
//  Serial.println(out);
  if (!allStop)
  {
    digitalWrite(emergencyStop, HIGH);
  }
}

void contact(int num, double d1, double d2, int sng)
{
  if (d2 < 0)
  {
//    Serial.print("near with ");
//    Serial.print(num);
//    Serial.print(" tenzo, force is ");
//    Serial.println(d1 - d2);
  }
  else
  {
//    Serial.print("between ");
//    Serial.print(num);
//    Serial.print(" and ");
//    Serial.print(num + sng);
//    Serial.print(" tenzo's, force is ");
//    Serial.print(d1 + d2);
//    Serial.print("on distance ");
//    Serial.print( (d1 / (d1 + d2)) * 47.0);  //47 - distance between  tenzo
//    Serial.print(" sm from tenzo ");
//    Serial.println(num);
  }
}

void loop()
{
  tenzo_dat();
}
