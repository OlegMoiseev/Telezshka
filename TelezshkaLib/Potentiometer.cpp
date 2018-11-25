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
	const double relativeAngle = 330.;
	const double absoluteAngle = 1024.;

	#ifdef POTENTIOMETER
		Serial.print("POTENTIOMETER called getAngle and anle = ");
		Serial.println(getData() * (relativeAngle / absoluteAngle));
	#endif

	return getData() * (relativeAngle / absoluteAngle);
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