#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "AnalogSensor.h"
#include "Defines.h"

class Potentiometer : public AnalogSensor
{
  public:
    Potentiometer(const int pin);
    double getAngle();

    Potentiometer& operator= (Potentiometer &some);
};

#endif