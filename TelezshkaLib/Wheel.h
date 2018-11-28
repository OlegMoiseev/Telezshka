#ifndef WHEEL_H
#define WHEEL_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "AngleMotor.h"
#include "SpeedMotor.h"
#include "Odometer.h"
#include "Defines.h"
#include <Array.h>

class Wheel
{
  private:
    AngleMotor _turnMotor;
    SpeedMotor _rollMotor;
    Odometer _wheelOdometer;
    double _needDistance;

  public:
    Wheel();
    Wheel(int* pinsForWheel, size_t size);
    void setMove(double spd, double angle, double distance, bool keyInterruption);
    Array <double, 2> wheelCurrentPosition();
    bool isTurnReached();
    void moveWheel(bool turning);
    bool isDistReached();
    double deltaDistance();
    
    Wheel& operator= (Wheel &&some);
};

#endif