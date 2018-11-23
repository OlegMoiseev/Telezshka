#ifndef ANALOGSENSOR_H
#define ANALOGSENSOR_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "Defines.h"

class AnalogSensor
{
  protected:
    int _pin;
    int getData();
  public:
    AnalogSensor(const int ASpin);
};

#endif