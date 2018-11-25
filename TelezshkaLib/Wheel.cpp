#include "Wheel.h"

Wheel::Wheel()
  :
  _turnMotor(44, 45, 46),//pins what we don't use
  _rollMotor(47, 48, 49),
  _needDistance(0),
  _wheelOdometer(_rollMotor._opto._odometer)
{
  #ifdef WHEEL
    Serial.println("WHEEL init empty");
  #endif
}

Wheel::Wheel(int pinsForWheel [6])
  :
  _turnMotor(pinsForWheel[1], pinsForWheel[4], pinsForWheel[5]),
  _rollMotor(pinsForWheel[0], pinsForWheel[2], pinsForWheel[3]),
  _needDistance(0),
  _wheelOdometer(_rollMotor._opto._odometer)
{
  #ifdef WHEEL
    Serial.println("WHEEL init with turnMotor, rollMotor, needDistance = 0, wheelOdometer");
  #endif
}

bool Wheel::isTurnReached()
{
  #ifdef WHEEL
    Serial.println("WHEEL called isTurnReached");
  #endif

  return _turnMotor.isAngleReached();
}

void Wheel::setMove(double angle, double spd, double distance, bool keyInterruption)
{
  #ifdef WHEEL
    Serial.print("WHEEL called setMove and angle = ");
    Serial.print(angle);
    Serial.print(" speed = ");
    Serial.print(spd);
    Serial.print(" distance = ");
    Serial.println(distance);
  #endif
  
  if (!keyInterruption)
  {
    _rollMotor.reset();
  }

  _turnMotor.setAngle(angle);
  _rollMotor.setSpd(spd);
  _needDistance = distance;
}

double Wheel::deltaDistance()
{
  #ifdef WHEEL
    Serial.println("WHEEL called deltaDistance");
  #endif
    // Serial.println(abs(_needDistance - _rollMotor._opto._odometer.getDistance()));
  return abs(_needDistance - _rollMotor._opto._odometer.getDistance());
}

Array <double, 2> Wheel::wheelCurrentPosition()
{
  #ifdef WHEEL
    Serial.println("WHEEL called wheelCurrentPosition");
  #endif

  Array <double, 2> current;
  current.at(0) = _rollMotor._opto._odometer.getDistance();
  current.at(1) = _turnMotor.getAngleNow();

  return current;
}

void Wheel::moveWheel(bool turning)
{
  #ifdef WHEEL
    Serial.println("WHEEL called moveWheel");
  #endif

  _turnMotor.standAngle();
  if (turning)
  {
    if (isDistReached())
    {
      _rollMotor.stopMove();
    }
    else
    {
      _rollMotor.standSpeed();
    }
  }
}

bool Wheel::isDistReached()
{
  #ifdef WHEEL
    Serial.println("WHEEL called isDistReached");
  #endif

  return deltaDistance() <= 5.;
}

Wheel& Wheel::operator= (Wheel &&some)
{
  #ifdef WHEEL
    Serial.println("WHEEL called operator=");
  #endif

  this->_wheelOdometer = some._wheelOdometer;
  this->_turnMotor = some._turnMotor;
  this->_rollMotor = some._rollMotor;
  this->_needDistance = some._needDistance;
  return *this;
}