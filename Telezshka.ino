#include <Telezshka.h>

int pFW1 [6] = {29, A1, 5, 25, 2, 22};
int pFW2 [6] = {28, A2, 6, 26, 3, 23};
int pFW3 [6] = {30, A3, 7, 27, 4, 24};

double xyz [3 * numberOfWheels];

unsigned long int iterations = 0;

const int interruptPin = 18;

bool interruption = false;
bool wrIte = false;
bool telegaMoving = false;

Telezshka *telega = nullptr;

void emergencyStopTelezshka()
{
//  Serial.println("iIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
  interruption = true;
  if (telegaMoving)
  {
    telega->stopMove(); 
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(100);
  Serial.println("Started");
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), emergencyStopTelezshka, FALLING);
  telega = new Telezshka(pFW1, pFW2, pFW3);
}


void printPositions(Telezshka* telega, int numberOfWheels)
{
  telega->updateCurrentPosition();
  for(int i = 0; i < 2*numberOfWheels; ++i)
  {
    Serial.print(telega->_positions[i]);
    Serial.print(' ');
  }
  Serial.println();
}

void loop()
{   
    ++iterations;
    if (Serial.available() > 0)
    {
      interruption = false;
      telegaMoving = true;
      wrIte = 1;
      iterations = 0; 
      
      for (int i = 0; i < 9; ++i)
      {
        xyz[i] = Serial.parseFloat();
      }

      telega->setGo(xyz);
      
      if ((xyz[2] + xyz[5] + xyz[8]) < 1.)
      {
        iterations = 0;
        printPositions(telega, numberOfWheels);
      }
    }

    if (wrIte && iterations % 500 == 0)
    {
      iterations = 0;
      printPositions(telega, numberOfWheels); 
    }
    
    telega->goTo();

    if (!interruption && telega->isReachedDistance())
    {
      wrIte = 0;
      iterations = 0;
      telegaMoving = false;
      printPositions(telega, numberOfWheels); 
      Serial.println("done");
    }
    if (interruption && telega->isReachedDistance())
    {
        wrIte = 0;
        iterations = 0;
        telegaMoving = false;
        printPositions(telega, numberOfWheels); 
        Serial.println("1");
    }
}

// 30.0 -200.0 200.0 -30.0 200.0 200.0 -90.0 -200.0 200.0
// 30.0 200.0 200.0 -30.0 -200.0 200.0 -90.0 200.0 200.0
// 30.0 0.0 0.0 -30.0 0.0 0.0 -90.0 0.0 0.0
// 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
// 0.0 150.0 500.0 0.0 150.0 500.0 0.0 150.0 500.0
// 0.0 255.0 100.0 0.0 255.0 100.0 0.0 255.0 100.0
// 0.0 -255.0 100.0 0.0 -255.0 100.0 0.0 -255.0 100.0
// 0.0 -255.0 1000.0 0.0 -255.0 1000.0 0.0 -255.0 1000.0
// 0.0 -255.0 400.0 0.0 -255.0 400.0 0.0 -255.0 400.0
// 0.0 255.0 400.0 0.0 255.0 400.0 0.0 255.0 400.0
