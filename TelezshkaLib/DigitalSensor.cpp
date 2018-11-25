#include "DigitalSensor.h"

DigitalSensor::DigitalSensor(const int pin)
  :
  _pin(pin)
{
	#ifdef DIGITALSENSOR
		Serial.print("DIGITALSENSOR init on pin = ");
		Serial.println(_pin);
	#endif

  	pinMode(_pin, INPUT);
}

bool DigitalSensor::getData()
{
	#ifdef DIGITALSENSOR
		Serial.print("DIGITALSENSOR called getData with data = ");
		Serial.println(digitalRead(_pin));
	#endif
  return digitalRead(_pin);
}