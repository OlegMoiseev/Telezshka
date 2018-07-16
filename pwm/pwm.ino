#include <TimerOne.h>


void setup()
{
  Timer1.initialize();
  Timer1.pwm(9, 512);
  pinMode(10, OUTPUT);
  Timer1.attachInterrupt(roll);
}

void roll()
{
  digitalWrite(10, !digitalRead(10));  
}

void loop()
{

}
