#include "AngleMotor.h"

AngleMotor::AngleMotor(const int potenPin, const int speedPin, const int reversePin)
  :
  _poten(Potentiometer(potenPin)),
  Motor(speedPin, reversePin),
  _turnKoefPID(1.4),
  _turnEps(1.),
  _needAngle(175.),
  _gearsKoef(94. / 75.),
  _angleReached(false),
  _angleRotatingLimitRelative(120.)
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
  if (abs(angle) > _angleRotatingLimitRelative)
  {
    if (angle > _angleRotatingLimitRelative)
    {
      angle = _angleRotatingLimitRelative;
    }
    if (angle < -_angleRotatingLimitRelative)
    {
      angle = -_angleRotatingLimitRelative;
    }
  }

  /****
  * this constant need to conform angle from relative (0, |120|) to absolute (0, 330) form
  ****/
  const double relativeToAbsolutelyAngle = 140.;

  _needAngle = (angle + relativeToAbsolutelyAngle) * _gearsKoef;

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

  /*****
  * For safety, we limit the operation area of ​​the potentiometer.
  *****/

  const double leftAngleLimit = 10.;
  const double rightAngleLimit = 320.;
  const double safetyRadius = 10.;


  if ((_currentAngle < leftAngleLimit) || (_currentAngle > rightAngleLimit))
  {
    /*****
    * We need to stop turning wheel when we in depth zone(in this zone the angle is not stable)
    *****/
    const double speedWhenWeInDepthZone = 0.;

    setRotationSpeed(speedWhenWeInDepthZone);
  }

  double spd;
  if ((_needAngle <= rightAngleLimit + safetyRadius) && (_needAngle >= leftAngleLimit - safetyRadius))
  {
    /*****
    * This koeficient is good for using PID in turning.
    *****/
    double minDelta = 30.;

    if (abs(_needAngle - _currentAngle) > _turnEps)
    {
      double delta = _currentAngle - _needAngle;
      if (abs(delta) < minDelta)
      {
        /*****
        * Calculating speed of turning in PID. Robot can set only positive speed value.
        *****/
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
      /*****
      * Set 0 speed when we on true angle.
      *****/
      spd = 0.;
      setRotationSpeed(spd);
      _angleReached = true;
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