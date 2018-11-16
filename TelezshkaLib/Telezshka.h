#ifndef TELEZSHKA_H
#define TELEZSHKA_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "Wheel.h"
#include "Defines.h"

const int numberOfWheels = 3;

class Telezshka
{
  private:
    Array<Wheel, numberOfWheels> _wheels;
    bool _doneMove;
    bool _doneTurn;
    double _stopArray [3 * numberOfWheels];
    
  public:
    double _positions [2 * numberOfWheels];
    Telezshka(int pinsForWheel1 [6], int pinsForWheel2 [6], int pinsForWheel3 [6]);
    Telezshka();
    void setGo(double valuesMove [3 * numberOfWheels]);
    void goTo();
    void stopMove();
    void updateCurrentPosition();
    bool isReachedDistance();
    
    Telezshka& operator= (Telezshka &some);
};

#endif