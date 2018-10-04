#include "Odometer.h"

Odometer::Odometer()
  :
  _distance(0)
{

}

void Odometer::resetOdometer()
{
  _distance = 0.;
}

void Odometer::updateDistance()
{
  _distance += 5.;
}

double Odometer::getDistance()
{
  return _distance;
}

Odometer& Odometer::operator= (Odometer &some)
{
  this->_distance = some._distance;
  return *this;
}