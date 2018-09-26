#ifndef TELEZSHKA_H
#define TELEZSHKA_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "Wheel.h"

const int numberOfWheels = 3;

class Telezshka
{
  private:
    Array<Wheel, numberOfWheels> _wheels;
    bool _doneMove;
    bool _doneTurn;
    double _stopArray [3 * numberOfWheels];
  public:
    Telezshka(int pinsForWheel1 [6], int pinsForWheel2 [6], int pinsForWheel3 [6]);
    Telezshka();
    void setGo(double valuesMove [3 * numberOfWheels]);
    void goTo();
    void stopMove();
    Array<double, 2 * numberOfWheels> telezshkaCurrentPosition();
    bool isReachedDistance();
    
    Telezshka& operator= (Telezshka &some);
};

#endif