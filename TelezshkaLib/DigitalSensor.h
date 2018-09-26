#ifndef DIGITALSENSOR_H
#define DIGITALSENSOR_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

class DigitalSensor
{
  protected:
    int _pin;
    int getData();
  public:
    DigitalSensor(const int pin);
};

#endif