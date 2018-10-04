#include "SpeedMotor.h"

SpeedMotor::SpeedMotor(const int optoPin, const int speedPin, const int reversePin)
  :
  _opto(OptoSensor(optoPin)),
  Motor(speedPin, reversePin),
  _needSpeed(0.),
  _sendSpd(0.)
{

}

void SpeedMotor::stopMove()
{
  setRotationSpeed(0.);
  _opto._odometer.resetOdometer();
}

void SpeedMotor::setSpd(double spd)
{
  _needSpeed = spd;
}

void SpeedMotor::standSpeed()
{
  if (abs(_needSpeed) < 1.)
  {
    stopMove();
  }
  else
  {
    double delta = abs(_needSpeed) - _opto.getSpeed();
    if (abs(delta) > 10.)
    {
      _sendSpd += (255. / 200.) * delta / 500.;  // 200 - maxSpeed 2.0 - koef PID
      if (_sendSpd < 0.)
      {
        _sendSpd = 50.;
      }
      if (_needSpeed > 0.)
      {
        setRotationSpeed(_sendSpd);
      }
      else
      {
        setRotationSpeed(-_sendSpd);
      }
    }
  }
}

SpeedMotor& SpeedMotor::operator= (SpeedMotor &some)
{
  this->_needSpeed = some._needSpeed;
  this->_currentSpeed = some._currentSpeed;
  this->_sendSpd = some._sendSpd;
  this->_speedPin = some._speedPin;
  this->_reversePin = some._reversePin;
  this->_opto = some._opto;
  return *this;
}