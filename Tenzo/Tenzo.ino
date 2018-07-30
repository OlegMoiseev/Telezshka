#include "HX711.h"
#include <Array.h>

#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
//#define DOUT1  5
//#define CLK1  4
const int DOUT2 = 3;
const int CLK2 = 2;

double epsForce = 3.0;
bool check = true;
int emergencyStop = 8;
Array<HX711, 9> tenzo;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(0);
  //for(int i = 0; i < 9; ++i)
  //{
    tenzo[0] = HX711(2, 3);
    tenzo[0].set_scale(calibration_factor);
    tenzo[0].tare();
  //}
  pinMode(emergencyStop, OUTPUT);
}
void tenzo_dat()
{
    double maxForce = 0.0;
    double maxForceSide; 
    int forceNum = -1;
    double force;
    //for(int i=0; i<9; i++)
    //{
      force = tenzo[0].get_units();
      Serial.println(force);
      if (maxForce < abs(force))
      {
      //  if (i % 3 == 0)
        //{
            maxForce = force;
            forceNum = 0;
//        }
//        if (i % 3 == 1)
//        {
//          maxForce = abs(force);
//          maxForceSide = tenzo[i+1].get_units();
//          forceNum = i;
//        }
//        if (i % 3 == 2)
//        {
//          maxForce = abs(force);
//          maxForceSide = tenzo[i-1].get_units();
//        }
        }
//    }
    if (maxForce > epsForce)
    {
      Serial.print("Collision! ");
      if (forceNum % 3 == 0)
      {
        Serial.print(forceNum);
        Serial.print(" tenzo, force is ");
        Serial.print(maxForce);
      }
      if (forceNum % 3 == 1)
      {
        contact(forceNum, maxForce, maxForceSide,1);
      }
      if (forceNum % 3 ==  2)
      {
        contact(forceNum, maxForce, maxForceSide,-1); 
      }
      digitalWrite(emergencyStop, LOW);
    }
    else
    {
      digitalWrite(emergencyStop, HIGH);
    }
    
}
 
void contact(int num, double d1, double d2, int sng)
{
  if (d2 < 0)
  {
    Serial.print("near with ");
    Serial.print(num);
    Serial.print(" tenzo, force is ");
    Serial.println(d1-d2); 
  }
  else
  {
    Serial.print("between ");
    Serial.print(num);
    Serial.print(" and ");
    Serial.print(num+sng);
    Serial.print(" tenzo's, force is ");
    Serial.print(d1+d2);
    Serial.print("on distance ");
    Serial.print( (d1 / (d1 + d2)) * 47.0);  //47 - distance between  tenzo
    Serial.print(" sm from tenzo ");
    Serial.println(num);  
  }
}

void loop() 
{
    tenzo_dat();
}
