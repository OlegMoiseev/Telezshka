#ifndef OPTOSENSOR_H
#define OPTOSENSOR_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "DigitalSensor.h"
#include "Odometer.h"
#include "Defines.h"

class OptoSensor : public DigitalSensor
{
  private:
    bool _prevState;
    bool _curState;
    unsigned long _prevTime;
    unsigned long _stepTime;
  public:
    Odometer _odometer;
    OptoSensor(const int pin);
    double getSpeed();

    OptoSensor& operator= (OptoSensor &some);
};

#endif