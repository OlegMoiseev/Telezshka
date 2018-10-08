#include "Telezshka.h"

Telezshka::Telezshka(int pinsForWheel1 [6], int pinsForWheel2 [6], int pinsForWheel3 [6])
  :
  _doneMove(false),
  _doneTurn(false)
{
  #ifdef TELEZSHKA
    Serial.print("TELEZSHKA init with arrays of wheels and stopArray with pins w1 = [ ");
    for (int i = 0; i < 6; i++)
    {
      Serial.print(pinsForWheel1[i]);
      Serial.print(" ");
    }
    Serial.print("], w2 = [ ");
    for (int i = 0; i < 6; i++)
    {
      Serial.print(pinsForWheel2[i]);
      Serial.print(" ");
    }
    Serial.print("], w3 = [ ");
    for (int i = 0; i < 6; i++)
    {
      Serial.print(pinsForWheel3[i]);
      Serial.print(" ");
    }
    Serial.print("], doneMove = ");
    Serial.print(_doneMove);
    Serial.print(", doneTurn = ");
    Serial.println(_doneTurn);
  #endif

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
  #ifdef TELEZSHKA
    Serial.print("TELEZSHKA called setGo with values = [ ");
    for (int i = 0; i < 3 * numberOfWheels; ++i)
    {
      Serial.print(valuesMove[i]);
      Serial.print(" ");
    }
    Serial.println("]");
  #endif

  _doneMove = true;
  _doneTurn = true;

  for (int i = 0; i < 3 * numberOfWheels; ++i)
  {
    _wheels.at(i).setMove(valuesMove[i * 3], valuesMove[i * 3 + 1], valuesMove[i * 3 + 2]);
  }
}

void Telezshka::goTo()
{
  #ifdef TELEZSHKA
    Serial.println("TELEZSHKA called goTo");
  #endif

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
  #ifdef TELEZSHKA
    Serial.println("TELEZSHKA called telezshkaCurrentPosition");
  #endif

  Array<double, 2 * numberOfWheels> positions;
  Array<double, 2> curWheel;
  
  for(int i = 0; i < numberOfWheels; i += 2)
  {
    curWheel = _wheels.at(i).wheelCurrentPosition();
  
    positions.at(i) = curWheel.at(0);
    positions.at(i + 1) = curWheel.at(1);
  }
  
  return positions;
}

bool Telezshka::isReachedDistance()
{
  #ifdef TELEZSHKA
    Serial.print("TELEZSHKA called isReachedDistance");
  #endif

  bool tmp = true;
  
  for (int i = 0; i < numberOfWheels; ++i)
  {
    tmp = tmp && _wheels.at(i).isDistReached();
  }

  if (_doneMove && tmp)
  {
    _doneMove = false;
    #ifdef TELEZSHKA
      Serial.println("with TRUE");
    #endif
    
    return true;
  }
  else
  {
    #ifdef TELEZSHKA
      Serial.println("with FALSE");
    #endif

    return false;
  }
}

void Telezshka::stopMove()
{
  #ifdef TELEZSHKA
    Serial.println("TELEZSHKA called stopMove");
  #endif

  setGo(_stopArray);
  goTo();
  telezshkaCurrentPosition();
}