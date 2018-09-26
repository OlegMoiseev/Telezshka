#include "OptoSensor.h"

OptoSensor::OptoSensor(const int pin)
  :
  _prevTime(micros()),
  DigitalSensor(pin),
  _odometer(Odometer())
{

}

double OptoSensor::getSpeed()
{
  _curState = getData();
  if (_curState != _prevState)
  {
    _stepTime = micros() - _prevTime;
    _prevState = _curState;
    _prevTime = micros();
    _curState == true ? _stepTime *= 1.382239382 : _stepTime *= 0.7306122449;
    _odometer.updateDistance();
    return 5. / (static_cast<double>(_stepTime) / 1e6);  // 5 mm / (time in microseconds / 1 000 000);
  }
}

OptoSensor& OptoSensor::operator= (OptoSensor &some)
{
  this->_pin = some._pin;
  this->_prevState = some._prevState;
  this->_curState = some._curState;
  this->_prevTime = some._prevTime;
  this->_stepTime = some._stepTime;
  this->_odometer = some._odometer;
  return *this;
}