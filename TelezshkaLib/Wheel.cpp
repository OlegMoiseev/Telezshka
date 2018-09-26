#include "Wheel.h"
Wheel::Wheel()
  :
  _turnMotor(44, 45, 46),//pins what we don't use
  _rollMotor(47, 48, 49),
  _needDistance(0),
  _wheelOdometer(_rollMotor._opto._odometer)
{

}

Wheel::Wheel(int pinsForWheel [6])
  :
  _turnMotor(pinsForWheel[1], pinsForWheel[4], pinsForWheel[5]),
  _rollMotor(pinsForWheel[0], pinsForWheel[2], pinsForWheel[3]),
  _needDistance(0),
  _wheelOdometer(_rollMotor._opto._odometer)
{

}

bool Wheel::isTurnReached()
{
  return _turnMotor.isAngleReached();
}

void Wheel::setMove(double angle, double spd, double distance)
{
  _turnMotor.setAngle(angle);
  _rollMotor.setSpd(spd);
  _needDistance = distance;
}

double Wheel::deltaDistance()
{
  return abs(_needDistance - _rollMotor._opto._odometer.getDistance());
}

Array <double, 2> Wheel::wheelCurrentPosition()
{
  Array <double, 2> current;
  current.at(0) = _wheelOdometer.getDistance();
  current.at(1) = _turnMotor.getAngleNow();
  return current;
}

void Wheel::moveWheel(bool turning)
{
  _turnMotor.standAngle();
  if (turning)
  {
    if (isDistReached())
    {
      _rollMotor.stopMove();
      _needDistance = 0;
    }
    else
    {
      _rollMotor.standSpeed();
    }
  }
}

bool Wheel::isDistReached()
{
  return deltaDistance() <= 5.;
}

Wheel& Wheel::operator= (Wheel &&some)
{
  this->_wheelOdometer = some._wheelOdometer;
  this->_turnMotor = some._turnMotor;
  this->_rollMotor = some._rollMotor;
  this->_needDistance = some._needDistance;
  return *this;
}