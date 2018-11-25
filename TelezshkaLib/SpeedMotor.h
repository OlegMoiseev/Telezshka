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

    /*****
    * Minimal speed when Telezshka can moving
    *****/
    const double _minimalSpeedWhenTelezshkaRiding;

    const double _absolutelyMaxSpeed;

    /*****
    * On what value of speed we must reacting.
    *****/
    const double _epsilonSpeed;

    /*****
    * How quickly we change rolling speed.
    *****/
    const double _numberOfStaps;
  
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