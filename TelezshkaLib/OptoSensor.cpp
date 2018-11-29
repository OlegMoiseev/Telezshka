#include "OptoSensor.h"

OptoSensor::OptoSensor(const int pin)
  :
  _prevTime(micros()),
  DigitalSensor(pin),
  _odometer(Odometer()),
  _prevState(!getData())
{
  #ifdef OPTOSENSOR
    Serial.print("OPTOSENSOR init with prevTime = ");
    Serial.print(_prevTime);
    Serial.print(" DIGITALSENSOR with pin = ");
    Serial.print(pin);
    Serial.println(" and ODOMETER");
  #endif
}

double OptoSensor::getSpeed()
{ 
  _curState = getData();
  if (_curState != _prevState)
  {

    /*****
    * Time what we need to pass by tooth and by space between two tooths.
    *****/
    const double stepInTrueCurState = 1.382239382;
    const double stepInFalseCurState = 0.7306122449; 
    const double convertMicrosecToSec = 1e6;

    _stepTime = micros() - _prevTime;
    _prevState = _curState;
    _prevTime = micros();
    _curState == true ? _stepTime *= stepInTrueCurState : _stepTime *= stepInFalseCurState;
    _odometer.updateDistance();

    #ifdef OPTOSENSOR
      Serial.print("OPTOSENSOR called getSpeed and speed = ");
      Serial.println(_odometer.getStep() / (static_cast<double>(_stepTime) / convertMicrosecToSec));
    #endif
      
    /*****
    * 5 mm / (time in microseconds / 1 000 000)
    *****/
    return _odometer.getStep() / (static_cast<double>(_stepTime) / convertMicrosecToSec);
  }
}

OptoSensor& OptoSensor::operator= (OptoSensor &some)
{
  #ifdef OPTOSENSOR
      Serial.print("OPTOSENSOR called operator= and pin = ");
      Serial.print(this->_pin);
      Serial.print(" -> ");
      Serial.print(some._pin);
      Serial.print(" prevState = ");
      Serial.print(this->_prevState);
      Serial.print(" -> ");
      Serial.print(some._prevState);
      Serial.print(" curState = ");
      Serial.print(this->_curState);
      Serial.print(" -> ");
      Serial.print(some._curState);
      Serial.print(" prevTime = ");
      Serial.print(this->_prevTime);
      Serial.print(" -> ");
      Serial.print(some._prevTime);
      Serial.print(" stepTime = ");
      Serial.print(this->_stepTime);
      Serial.print(" -> ");
      Serial.print(some._stepTime);
      Serial.println(" and ODOMETER");
  #endif
      
  this->_pin = some._pin;
  this->_prevState = some._prevState;
  this->_curState = some._curState;
  this->_prevTime = some._prevTime;
  this->_stepTime = some._stepTime;
  this->_odometer = some._odometer;
  return *this;
}
