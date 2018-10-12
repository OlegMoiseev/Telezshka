#include <Telezshka.h>

int pFW1 [6] = {29, A1, 5, 25, 2, 22};
int pFW2 [6] = {28, A2, 6, 26, 3, 23};
int pFW3 [6] = {30, A3, 7, 27, 4, 24};

double xyz [3 * numberOfWheels];

Telezshka *telega = nullptr;

void setup()
{
  Serial.begin(2000000);
  Serial.setTimeout(100);
  Serial.println("Started");
  telega = new Telezshka(pFW1, pFW2, pFW3);
}
 
// 30.0 0.0 0.0 -30.0 0.0 0.0 -90.0 0.0 0.0
// 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
// 0.0 150.0 500.0 0.0 150.0 500.0 0.0 150.0 500.0
// 0.0 255.0 100.0 0.0 255.0 100.0 0.0 255.0 100.0
// 0.0 -255.0 100.0 0.0 -255.0 100.0 0.0 -255.0 100.0

void loop()
{   
    if (Serial.available() > 0)
    {
      for (int i = 0; i < 9; ++i)
      {
        xyz[i] = Serial.parseFloat(); 
      }
      
      telega->setGo(xyz);
      
      if ((xyz[2] + xyz[5] + xyz[8]) < 1.)
      {
        telega->telezshkaCurrentPosition();
      }
    }
    
    telega->goTo();
    
    if (telega->isReachedDistance())
    {
      Serial.print("done");
    }
}
