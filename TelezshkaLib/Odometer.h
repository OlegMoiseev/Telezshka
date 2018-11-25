#ifndef ODOMETER_H
#define ODOMETER_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "Defines.h"

class Odometer
{
  private:
    double _distance;

    /*****
    * Passing each tooth turns the wheel 5 mm.
    *****/
    double _step;
  
  public:
    Odometer();
    void resetOdometer();
    void updateDistance();
    double getDistance();
    double getStep();

    Odometer& operator= (Odometer &some);
};

#endif