#ifndef MOTOR_H
#define MOTOR_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "Defines.h"

class Motor
{
  protected:
    int _speedPin;
    int _reversePin; 
  
  private:
    void rollForward();
    void rollBackward();
   
  public:
    void setRotationSpeed(double spd);
    Motor(const int speedPin, const int reversePin);
};

#endif