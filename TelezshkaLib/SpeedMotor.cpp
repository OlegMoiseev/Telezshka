#include "SpeedMotor.h"

SpeedMotor::SpeedMotor(const int optoPin, const int speedPin, const int reversePin)
  :
  _opto(OptoSensor(optoPin)),
  Motor(speedPin, reversePin),
  _needSpeed(0.),
  _maxSpeed(200.),
  _numberOfStaps(500.),
  _sendSpd(0.),
  _absolutelyMaxSpeed(255.),
  _epsilonSpeed(10.),
  _minimalSpeedWhenTelezshkaRiding(50.)
{
  #ifdef SPEEDMOTOR
    Serial.print("SPEEDMOTOR init: OptoSensor ");
    Serial.print(optoPin);
    Serial.print(" Motor ");
    Serial.print(speedPin);
    Serial.print(", ");
    Serial.print(reversePin);
    Serial.print(", needSpeed ");
    Serial.print(_needSpeed);
    Serial.print(", sendSpd ");
    Serial.println(_sendSpd);
  #endif
}

void SpeedMotor::stopMove()
{
  #ifdef SPEEDMOTOR
    Serial.println("SPEEDMOTOR called stopMove");
  #endif

  setRotationSpeed(0.);
}

void SpeedMotor::reset()
{
  _opto._odometer.resetOdometer();
}

void SpeedMotor::setSpd(double spd)
{
  #ifdef SPEEDMOTOR
    Serial.print("SPEEDMOTOR called setSpd with speed = ");
    Serial.println(spd);
  #endif

  _needSpeed = spd;
}

void SpeedMotor::standSpeed()
{
  const double insignificantSpeed = 1.;
  if (abs(_needSpeed) < insignificantSpeed)
  {
    stopMove();
  }
  else
  {
    double delta = abs(_needSpeed) - _opto.getSpeed();
    if (abs(delta) > _epsilonSpeed)
    {
      /*****
      * 200 - maxSpeed 2.0 - koef PID
      * We can calculate a negative value, so we set in this case a minimal speed.
      *****/
      _sendSpd += (_absolutelyMaxSpeed / _maxSpeed) * delta / _numberOfStaps;
      if (_sendSpd < 0.)
      {
        _sendSpd = _minimalSpeedWhenTelezshkaRiding;
      }
      _needSpeed > 0. ? setRotationSpeed(_sendSpd) : setRotationSpeed(-_sendSpd);
    }
  }

  #ifdef SPEEDMOTOR
    Serial.print("SPEEDMOTOR called standSpeed with speed = ");
    Serial.println(_sendSpd);
  #endif
}

SpeedMotor& SpeedMotor::operator= (SpeedMotor &some)
{
  #ifdef SPEEDMOTOR
    Serial.print("SPEEDMOTOR called operator= with needSpeed = ");
    Serial.print(this->_needSpeed);
    Serial.print(" -> ");
    Serial.print(some._needSpeed);
    Serial.print(" currentSpeed = ");
    Serial.print(this->_currentSpeed);
    Serial.print(" -> ");
    Serial.print(some._currentSpeed);
    Serial.print(" sendSpd = ");
    Serial.print(this->_sendSpd);
    Serial.print(" -> ");
    Serial.print(some._sendSpd);
    Serial.print(" speedPin = ");
    Serial.print(this->_speedPin);
    Serial.print(" -> ");
    Serial.print(some._speedPin);
    Serial.print(" reversePin = ");
    Serial.print(this->_reversePin);
    Serial.print(" -> ");
    Serial.print(some._reversePin);
    Serial.println("and OptoSensor");
  #endif

  this->_needSpeed = some._needSpeed;
  this->_currentSpeed = some._currentSpeed;
  this->_sendSpd = some._sendSpd;
  this->_speedPin = some._speedPin;
  this->_reversePin = some._reversePin;
  this->_opto = some._opto;
  return *this;
}