#include "HX711.h"
#include <Array.h>
#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DOUT1  3
#define CLK1  2
//#define DOUT2 5
//#define CLK2 4
Array<double, 9> dat1, dat2;
double maxForce = 10.0;
bool check = true;

HX711 scale1(DOUT1, CLK1);
//HX711 scale2(DOUT2, CLK2);
void setup() {
  Serial.begin(9600);
  Serial.println("HX711 scale demo");
  scale1.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  //scale2.set_scale(calibration_factor);
  scale1.tare(); //Assuming there is no weight on the sca                    e at start up, reset the scale to 0
  //scale2.tare();
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
    for(int i = 0; i < 9; ++i)
    {
      dat2.at(i) = dat1.at(i);
    }
    dat1.at(0)=scale1.get_units();
    //dat1.at(1)=scale2.get_units();
    //dat1.at(2)=scale3.get_units();
    //dat1.at(3)=scale4.get_units();
    //dat1.at(4)=scale5.get_units();
    //dat1.at(5)=scale6.get_units();
    //dat1.at(6)=scale7.get_units();
    //dat1.at(7)=scale9.get_units();
    //dat1.at(8)=scale10.get_units();
    //for(int i = 0; i < 9; ++i)
    //{
      Serial.println(dat1.at(0));
    //}
    if ((abs(dat1.at(0) - dat2.at(0)) > maxForce) || (abs(dat1.at(1) - dat2.at(1)) > maxForce) || (abs(dat1.at(2) - dat2.at(2)) > maxForce) || (abs(dat1.at(3) - dat2.at(3)) > maxForce) || (abs(dat1.at(4) - dat2.at(4)) > maxForce) || (abs(dat1.at(5) - dat2.at(5)) > maxForce) || (abs(dat1.at(6) - dat2.at(6)) > maxForce) || (abs(dat1.at(7) - dat2.at(7)) > maxForce) || (abs(dat1.at(8) - dat2.at(8)) > maxForce)) 
    {
      Serial.println("Collision! I am stopping!");
      stopTelejka();
    }
    else
    {
      Serial.println("It's okey!");
    }
}
void loop() 
{
  dat1.at(0)=scale1.get_units();
  Serial.println(dat1.at(0));
  //if (check)
  //{
    //tenzo_dat();
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
