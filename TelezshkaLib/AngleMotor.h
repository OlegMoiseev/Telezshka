#ifndef ANGLEMOTOR_H
#define ANGLEMOTOR_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif


#include "Motor.h"
#include "Potentiometer.h"
#include "Defines.h"

class AngleMotor : public Motor
{
  private:
    bool _angleReached;
    double _needAngle;
    double _currentAngle;
    double _turnKoefPID;
    double _turnEps;
    double _gearsKoef;
    double _angleRotatingLimitRelative;
  public:
    Potentiometer _poten;
    AngleMotor(const int potenPin, const int speedPin, const int reversePin);
    void standAngle();
    void setAngle(double angle);
    bool isAngleReached();
    double getAngleNow();

    AngleMotor& operator= (AngleMotor &some);
};

#endif