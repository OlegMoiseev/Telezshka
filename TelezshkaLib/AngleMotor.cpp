#include "AngleMotor.h"

AngleMotor::AngleMotor(const int potenPin, const int speedPin, const int reversePin)
  :
  _poten(Potentiometer(potenPin)),
  Motor(speedPin, reversePin),
  _turnKoefPID(1.4),
  _turnEps(5.0),
  _needAngle(175.0),
  _gearsKoef(94. / 75.),
  _angleReached(false)
{

}

AngleMotor& AngleMotor::operator= (AngleMotor &some)
{
  this->_needAngle = some._needAngle;
  this->_currentAngle = some._currentAngle;
  this->_angleReached = some._angleReached;
  this->_turnKoefPID = some._turnKoefPID;
  this->_turnEps = some._turnEps;
  this->_gearsKoef = some._gearsKoef;
  this->_poten = some._poten;
  this->_speedPin = some._speedPin;
  this->_reversePin = some._reversePin;
  return *this;
}

void AngleMotor::setAngle(double angle)
{
  _angleReached = false;
  if (abs(angle) > 120)
  {
    if (angle > 120)
    {
      angle = 120;
    }
    if (angle < -120)
    {
      angle = -120;
    }
  }
  _needAngle = (angle + 140) * _gearsKoef;
}

double AngleMotor::getAngleNow()
{
  return _currentAngle;
}

void AngleMotor::standAngle()
{
  _currentAngle = _poten.getAngle();
  if ((_currentAngle < 10) || (_currentAngle > 320))
  {
    setRotationSpeed(0);
  }

  if ((_needAngle <= 330) && (_needAngle >= 0))
  {
    double spd;
    double minDelta = 30;

    if (abs(_needAngle - _currentAngle) > _turnEps)
    {
      double delta = _currentAngle - _needAngle;
      if (abs(delta) < minDelta)
      {
        delta > 0. ? spd = minDelta * _turnKoefPID : spd = -minDelta * _turnKoefPID;
      }
      else
      {
        spd = delta * _turnKoefPID;
      }
      setRotationSpeed(spd);
    }
    else
    {
      setRotationSpeed(0.0);
      _angleReached = true;
      //Serial.println("Stopped!");
    }
  }
}

bool AngleMotor::isAngleReached()
{
  return _angleReached;
}