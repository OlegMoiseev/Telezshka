#include "Potentiometer.h"

Potentiometer::Potentiometer(const int pin)
  :
  AnalogSensor(pin)
{

}

double Potentiometer::getAngle()
{
  return getData() * (330. / 1024.);
}

Potentiometer& Potentiometer::operator= (Potentiometer &some)
{
  this->_pin = some._pin;
  return *this;
}