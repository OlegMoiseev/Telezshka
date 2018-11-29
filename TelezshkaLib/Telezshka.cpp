#include "Telezshka.h"

Telezshka::Telezshka(int pinsForWheel1 [6], int pinsForWheel2 [6], int pinsForWheel3 [6])
  :
  _doneMove(false),
  _doneTurn(false),
  _keyInterruption(false)
{
  #ifdef TELEZSHKA
    Serial.print("TELEZSHKA init with arrays of wheels and stopArray with pins w1 = [ ");
    for (int i = 0; i < 2 * numberOfWheels; i++)
    {
      Serial.print(pinsForWheel1[i]);
      Serial.print(" ");
    }
    Serial.print("], w2 = [ ");
    for (int i = 0; i < 2 * numberOfWheels; i++)
    {
      Serial.print(pinsForWheel2[i]);
      Serial.print(" ");
    }
    Serial.print("], w3 = [ ");
    for (int i = 0; i < 2 * numberOfWheels; i++)
    {
      Serial.print(pinsForWheel3[i]);
      Serial.print(" ");
    }
    Serial.print("], doneMove = ");
    Serial.print(_doneMove);
    Serial.print(", doneTurn = ");
    Serial.println(_doneTurn);
  #endif


  /*****
  *CRUTCH
  *****/
  _wheels.at(0) = Wheel(pinsForWheel1);
  _wheels.at(1) = Wheel(pinsForWheel2);
  _wheels.at(2) = Wheel(pinsForWheel3);
  
  const double zero = 0.;
  for (int i = 0; i < numberOfWheels; ++i)
  {
    _memory[3 * i] = zero;
    _memory[3 * i + 1] = zero;
    _memory[3 * i + 2] = zero; 

    _stopArray[3 * i] = zero;
    _stopArray[3 * i + 1] = zero;
    _stopArray[3 * i + 2] = zero; 
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
  /*****
  * Telezska moving(turning/rolling)
  *****/
  _doneMove = true;
  /*****
  * Telezshka turning wheels(turning)
  *****/
  _doneTurn = true;

  double sum = 0.;
  
  for(int i = 0; i < 3 * numberOfWheels; ++i)
  {
    sum += abs(valuesMove[i]);
  }

  for (int i = 0; i < numberOfWheels; ++i)
  {
    if (sum < 1.)
    {
      _keyInterruption = true;
      _wheels.at(i).setMove(valuesMove[i * 3], valuesMove[i * 3 + 1], valuesMove[i * 3 + 2], _keyInterruption);
    }
    else
    {
      _memory[i * 3] = valuesMove[i * 3];
      _memory[i * 3 + 1] = valuesMove[i * 3 + 1];
      _memory[i * 3 + 2] = valuesMove[i * 3 + 2];

      _keyInterruption = false;
      _wheels.at(i).setMove(valuesMove[i * 3], valuesMove[i * 3 + 1], valuesMove[i * 3 + 2], _keyInterruption);
    }
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

void Telezshka::updateCurrentPosition()
{
  #ifdef TELEZSHKA
    Serial.println("TELEZSHKA called telezshkaCurrentPosition");
  #endif

  Array<double, 2> curWheel;

  int j = 0;
  for(int i = 0; i < numberOfWheels; ++i)
  {
    curWheel = _wheels.at(i).wheelCurrentPosition();
  
    _positions[j] = curWheel.at(0);
    _positions[j + 1] = curWheel.at(1);
    j += 2;
  }
}

bool Telezshka::isReachedDistance()
{
  #ifdef TELEZSHKA
    Serial.print("TELEZSHKA called isReachedDistance");
  #endif

  if (_doneMove && _keyInterruption)
  {
    _doneMove = false;
    return true;
  }

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
}

void Telezshka::setDataInMemory()
{

  _doneTurn = true;
  _doneMove = true;
  _keyInterruption = false;
  for (int i = 0; i < numberOfWheels; ++i)
  {
    _wheels.at(i).setMove(_memory[i * 3], _memory[i * 3 + 1], _memory[i * 3 + 2], true);
  }
  
}