#include "AnalogSensor.h"

AnalogSensor::AnalogSensor(const int pin)
  :
  _pin(pin)
{
	#ifndef ANALOGSENSOR
		Serial.print("ANALOGSENSOR init on pin");
		Serial.println(_pin);
	#endif
  	pinMode(_pin, INPUT);
}

int AnalogSensor::getData()
{
	#ifndef ANALOGSENSOR
		Serial.print("ANALOGSENSOR called getData with data = ");
		Serial.println(analogRead(_pin));
	#endif
  	return analogRead(_pin);
}