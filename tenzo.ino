#include "HX711.h"
#include <Array.h>
#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
//#define DOUT1  5
//#define CLK1  4
#define DOUT2 3
#define CLK2 2
Array<double, 9> dat1, dat2;
double epsForce = 3.0;
bool check = true;

//HX711 scale2(DOUT1, CLK1);
HX711 scale3(DOUT2, CLK2);
void setup() {
  Serial.begin(115200);
  Serial.println("HX711 scale demo");
  //scale2.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale3.set_scale(calibration_factor);
  //scale2.tare(); //Assuming there is no weight on the sca                    e at start up, reset the scale to 0
  scale3.tare();
  for(int i=0; i<9; ++i)
  {
    dat1.at(i)=0;
    dat2.at(i)=0;
  }
  Serial.println("Readings:");
}
void stopTelejka()
{
  check = false;
  //moveWheel(0,);
  //moveWheel(0,);
  //moveWheel(0,);
}
void tenzo_dat()
{
    double maxForce = 0.0, maxForceSide; 
    int forceNum = -1;
    for(int i = 0; i < 9; ++i)
    {
      dat2.at(i) = dat1.at(i);
    }
    //dat1.at(0) = scale1.get_units();
    //if (maxForce < abs(dat1.at(0) - dat2.at(0)))
    //{
      //maxForce = abs(dat1.at(0) - dat2.at(X§X§0));
      //forceNum = 0;
    //}
//    dat1.at(1)=scale2.get_units();
//    if (maxForce < abs(dat1.at(1) - dat2.at(1)))
//    {
//      maxForce = abs(dat1.at(1) - dat2.at(1));
//      maxForceSide = dat1.at(2) - dat2.at(2);
//      forceNum = 1;    
//    }
    dat1.at(2)=scale3.get_units();
    if (maxForce < abs(dat1.at(2) - dat2.at(2)))
    {
      maxForce = abs(dat1.at(2) - dat2.at(2));
      maxForceSide = dat1.at(1) - dat2.at(1);
      forceNum = 2;
    }
//    dat1.at(3)=scale4.get_units();
//    if (maxForce < abs(dat1.at(3) - dat2.at(3)))
//    {
//      maxForce = abs(dat1.at(3) - dat2.at(3));
//      forceNum = 3;
//    }
//    dat1.at(4)=scale5.get_units();
//    if (maxForce < abs(dat1.at(4) - dat2.at(4)))
//    {
//      maxForce = abs(dat1.at(4) - dat2.at(4));
//      maxForceSide = dat1.at(5) - dat2.at(5);
//      forceNum = 4;
//    }
//    dat1.at(5)=scale6.get_units();
//    if (maxForce < abs(dat1.at(5) - dat2.at(5)))
//    {
//      maxForce = abs(dat1.at(5) - dat2.at(5));
//      maxForceSide = dat1.at(4) - dat2.at(4);
//      forceNum = 5;
//    }
//    dat1.at(6)=scale7.get_units();
//    if (maxForce < abs(dat1.at(6) - dat2.at(6)))
//    {
//      maxForce = dat1.at(6) - dat2.at(6);
//      forceNum = 6;
//    }
//    dat1.at(7)=scale9.get_units();
//    if (maxForce < abs(dat1.at(7) - dat2.at(7)))
//    {
//      maxForce = abs(dat1.at(7) - dat2.at(7));
//      maxForceSide = dat1.at(8) - dat.at(8); 
//      forceNum = 7;
//    }
//    dat1.at(8)=scale10.get_units();
//    if (maxForce < abs(dat1.at(8) - dat2.at(8)))
//    {
//      maxForce = abs(dat1.at(8) - dat2.at(8));
//      maxForceSide = dat1.at(7) - dat2.at(7);  
//      forceNum = 8;
//    }
    //for(int i = 0; i < 9; ++i)
    //{
//      Serial.print(dat1.at(1));
//      Serial.print("    ");   
      Serial.println(dat1.at(2));
    //}
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
        contact(forceNum, maxForce, maxForceSide,1,dat1.at(forceNum),dat1.at(forceNum+1));
      }
      if (forceNum % 3 ==  2)
      {
        contact(forceNum, maxForce, maxForceSide,-1,dat1.at(forceNum),dat1.at(forceNum-1)); 
      }
      stopTelejka();
    }
    else
    {
      //Serial.println("It's okey!"); 
    }
}
 
void contact(int num, double d1, double d2, int sng,double  force1, double force2)
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
    Serial.print( (force2 / (force1 + force2)) * 47.0);  //47 - distance between  tenzo
    Serial.print(" sm from tenzo ");
    Serial.println(num);  
  }
}

void loop() 
{
  //dat1.at(0)=scale1.get_units();
  //dat1.at(1)=scale2.get_units();
  //Serial.print(dat1.at(0)/16.5*9.8);
  //Serial.print("     ");
  //Serial.println(dat1.at(1)/16.5*9.8);
  //if (check)
  //{
    tenzo_dat();
    delay(100);
  //}
  //double dat2=scale2.get_units();
  //Serial.println(dat1);
  //Serial.print(" ");
  //Serial.println(dat2); 
  //delay(100);
  //double dat2=scale.get_units();
  //if (abs(dat2 - dat1)>maxForce)
  //{
    //Serial.println(abs(dat2 - dat1));
  //}
}
