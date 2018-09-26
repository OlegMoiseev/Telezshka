#include "Motor.h"

Motor::Motor(const int speedPin, const int reversePin)
  :
  _speedPin(speedPin),
  _reversePin(reversePin)
{
  pinMode(_speedPin, OUTPUT);
  pinMode(_reversePin, OUTPUT);
}

void Motor::rollForward()
{
  digitalWrite(_reversePin, LOW);
}

void Motor::rollBackward()
{
  digitalWrite(_reversePin, HIGH);
}

void Motor::setRotationSpeed(double spd)
{
  spd > 0 ? rollBackward() : rollForward();
  abs(spd) > 255 ? analogWrite(_speedPin, 255) : analogWrite(_speedPin, abs(spd));
}