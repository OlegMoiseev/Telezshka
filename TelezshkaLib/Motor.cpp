#include "Motor.h"

Motor::Motor(const int speedPin, const int reversePin)
  :
  _speedPin(speedPin),
  _reversePin(reversePin)
{
	#ifdef MOTOR
		Serial.print("MOTOR init with pins speedPin = ");
		Serial.print(_speedPin);
		Serial.print(" reversePin = ");
		Serial.println(_reversePin);
	#endif

  	pinMode(_speedPin, OUTPUT);
  	pinMode(_reversePin, OUTPUT);
}

void Motor::rollForward()
{
	#ifdef MOTOR
		Serial.print("MOTOR called rollForward and reversePin = ");
		Serial.print(_reversePin);
		Serial.print(" -> ");
		Serial.println(0);
	#endif
  digitalWrite(_reversePin, LOW);
}

void Motor::rollBackward()
{
	#ifdef MOTOR
		Serial.print("MOTOR called rollBackward and reversePin = ");
		Serial.print(_reversePin);
		Serial.print(" -> ");
		Serial.println(1);
	#endif
  digitalWrite(_reversePin, HIGH);
}

void Motor::setRotationSpeed(double spd)
{
	#ifdef MOTOR
		Serial.print("MOTOR called setRotationSpeed with speed = ");
		Serial.println(spd);
	#endif
  spd > 0. ? rollBackward() : rollForward();
  abs(spd) > 255. ? analogWrite(_speedPin, 255.) : analogWrite(_speedPin, abs(spd));
}