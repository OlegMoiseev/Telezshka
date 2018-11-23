#ifndef SPEEDMOTOR_H
#define SPEEDMOTOR_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "Motor.h"
#include "OptoSensor.h"
#include "Defines.h"

class SpeedMotor : public Motor
{
  private:
    double _needSpeed;
    double _currentSpeed;
    double _sendSpd;
    double _maxSpeed;
    double _numberOfStaps;
  
  public:
    OptoSensor _opto;
    SpeedMotor(const int optoPin, const int speedPin, const int reversePin);
    void standSpeed();
    void setSpd(double spd);
    void stopMove();
    void reset();
    
    SpeedMotor& operator= (SpeedMotor &some);
};

#endif