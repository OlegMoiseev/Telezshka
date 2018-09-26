#include "AnalogSensor.h"

AnalogSensor::AnalogSensor(const int pin)
  :
  _pin(pin)
{
  pinMode(_pin, INPUT);
}

int AnalogSensor::getData()
{
  return analogRead(_pin);
}