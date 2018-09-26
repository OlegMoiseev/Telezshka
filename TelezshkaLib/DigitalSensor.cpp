#include "DigitalSensor.h"

DigitalSensor::DigitalSensor(const int pin)
  :
  _pin(pin)
{
  pinMode(_pin, INPUT);
}

int DigitalSensor::getData()
{
  return digitalRead(_pin);
}