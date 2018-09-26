#include "Telezshka.h"

Telezshka::Telezshka(int pinsForWheel1 [6], int pinsForWheel2 [6], int pinsForWheel3 [6])
  :
  _doneMove(false),
  _doneTurn(false)
{
  _wheels.at(0) = Wheel(pinsForWheel1);
  _wheels.at(1) = Wheel(pinsForWheel2);
  _wheels.at(2) = Wheel(pinsForWheel3);
  
  for (int i = 0; i < numberOfWheels; ++i)
  {
    _stopArray[3 * i] = 0.;
    _stopArray[3 * i + 1] = 0.;
    _stopArray[3 * i + 2] = 0.; 
  }
}


void Telezshka::setGo(double valuesMove [3 * numberOfWheels])
{
  _doneMove = true;
  _doneTurn = true;

  for (int i = 0; i < 3 * numberOfWheels; ++i)
  {
    _wheels.at(i).setMove(valuesMove[i * 3], valuesMove[i * 3 + 1], valuesMove[i * 3 + 2]);
  }
}

void Telezshka::goTo()
{
  bool rollWheels = true;
  for (int i = 0; i < numberOfWheels; ++i)
  {
    rollWheels = rollWheels && _wheels.at(i).isTurnReached();
  }

  for (int i = 0; i < numberOfWheels; ++i)
  {
    _wheels.at(i).moveWheel(rollWheels);
  }
}

Array<double, 2 * numberOfWheels> Telezshka::telezshkaCurrentPosition()
{
  Array<double, 2 * numberOfWheels> positions;
  Array<double, 2> curWheel;
  
  for(int i = 0; i < numberOfWheels; i += 2)
  {
    curWheel = _wheels.at(i).wheelCurrentPosition();
  
    positions.at(i) = curWheel.at(0);
    positions.at(i + 1) = curWheel.at(0);
  }
  
  return positions;
}

bool Telezshka::isReachedDistance()
{
  bool tmp = true;
  
  for (int i = 0; i < numberOfWheels; ++i)
  {
    tmp = tmp && _wheels.at(i).isDistReached();
  }

  if (_doneMove && tmp)
  {
    _doneMove = false;
    return true;
  }
  else
  {
    return false;
  }
}

void Telezshka::stopMove()
{
  setGo(_stopArray);
  goTo();
  telezshkaCurrentPosition();
}