#include "AngleMotor.h"

AngleMotor::AngleMotor(const int potenPin, const int speedPin, const int reversePin)
  :
  _poten(Potentiometer(potenPin)),
  Motor(speedPin, reversePin),
  _turnKoefPID(1.4),
  _turnEps(1.),
  _needAngle(175.),
  _gearsKoef(94. / 75.),
  _angleReached(false)
{
  #ifdef ANGLEMOTOR
    Serial.print("ANGLEMOTOR init: Potentiometer ");
    Serial.print(potenPin);
    Serial.print(", Motor ");
    Serial.print(speedPin);
    Serial.print(" ");
    Serial.print(reversePin);
    Serial.print(", turnKoefPID ");
    Serial.print(_turnKoefPID);
    Serial.print(",turnEps ");
    Serial.print(_turnEps);
    Serial.print(", needAngle ");
    Serial.print(_needAngle);
    Serial.print(", gearsKoef ");
    Serial.print(_gearsKoef);
    Serial.print(", angleReached ");
    Serial.println(_angleReached);
  #endif
}

AngleMotor& AngleMotor::operator= (AngleMotor &some)
{
  #ifdef ANGLEMOTOR
    Serial.print("ANGLEMOTOR called operator= with pins(Motor(2)) ");
    Serial.print(this->_speedPin);
    Serial.print(" to ");
    Serial.print(some._speedPin);
    Serial.print(" ");
    Serial.print(this->_reversePin);
    Serial.print(" to ");
    Serial.println(some._reversePin);
  #endif
  
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
  if (abs(angle) > 120.)
  {
    if (angle > 120.)
    {
      angle = 120.;
    }
    if (angle < -120.)
    {
      angle = -120.;
    }
  }
  _needAngle = (angle + 140.) * _gearsKoef;

  #ifdef ANGLEMOTOR
    Serial.print("ANGLEMOTOR called setAngle with angle = ");
    Serial.print(angle);
    Serial.print(" and needAngle = ");
    Serial.println(_needAngle);
  #endif
}

double AngleMotor::getAngleNow()
{
  #ifdef ANGLEMOTOR
    Serial.print("ANGLEMOTOR called getAngleNow with currentAngle = ");
    Serial.println(_currentAngle);
  #endif

  return _currentAngle;
}

void AngleMotor::standAngle()
{
  _currentAngle = _poten.getAngle();
  if ((_currentAngle < 10.) || (_currentAngle > 320.))
  {
    setRotationSpeed(0.);
  }

  if ((_needAngle <= 330.) && (_needAngle >= 0.))
  {
    double spd;
    double minDelta = 30.;

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
      spd = 0.;
      setRotationSpeed(spd);
      _angleReached = true;
      //Serial.println("Stopped!");
    }
  }

  #ifdef ANGLEMOTOR
    Serial.print("ANGLEMOTOR called standAngle with currentAngle = ");
    Serial.print(_currentAngle);
    Serial.print(", needAngle = ");
    Serial.print(_needAngle);
    Serial.print(", speed = ");
    Serial.println(spd);
  #endif
}

bool AngleMotor::isAngleReached()
{
  #ifdef ANGLEMOTOR
    Serial.print("ANGLEMOTOR called isAngleReached with angleReached = ");
    Serial.println(_angleReached);
  #endif
    
  return _angleReached;
}