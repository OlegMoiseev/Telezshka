#include "Potentiometer.h"

Potentiometer::Potentiometer(const int pin)
  :
  AnalogSensor(pin)
{
	#ifdef POTENTIOMETER
		Serial.print("POTENTIOMETER init with pin = ");
		Serial.println(pin);
	#endif
}

double Potentiometer::getAngle()
{
	#ifdef POTENTIOMETER
		Serial.print("POTENTIOMETER called getAngle and anle = ");
		Serial.println(getData() * (330. / 1024.));
	#endif

  return getData() * (330. / 1024.);
}

Potentiometer& Potentiometer::operator= (Potentiometer &some)
{
	#ifdef POTENTIOMETER
		Serial.print("POTENTIOMETER called operator= and pin = ");
		Serial.print(this->_pin);
		Serial.print(" -> ");
		Serial.print(some._pin);
	#endif
		
  this->_pin = some._pin;
  return *this;
}