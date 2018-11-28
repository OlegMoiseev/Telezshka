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
    bool _keyInterruption;
    double _stopArray [3 * numberOfWheels];
    double _memory [3 * numberOfWheels];

  public:
    double _positions [2 * numberOfWheels];
    Telezshka(int* pinsForWheel1, size_t sizeW1, int* pinsForWheel2, size_t sizeW2, int* pinsForWheel3, size_t sizeW3);
    Telezshka();
    void setGo(double* valuesMove, size_t size);
    void goTo();
    void stopMove();
    void updateCurrentPosition();
    bool isReachedDistance();
    void setDataInMemory();
    
    Telezshka& operator= (Telezshka &some);
};

#endif