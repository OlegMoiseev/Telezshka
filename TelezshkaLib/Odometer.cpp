#include "Odometer.h"

Odometer::Odometer()
  :
  _distance(0)
{
	#ifdef ODOMETER
		Serial.print("ODOMETER init with distance = ");
		Serial.println(_distance);
	#endif
}

void Odometer::resetOdometer()
{
  _distance = 0.;

	#ifdef ODOMETER
		Serial.print("ODOMETER called resetOdometer and distance = ");
		Serial.println(_distance);
	#endif
}

void Odometer::updateDistance()
{
  _distance += 5.;

	#ifdef ODOMETER
		Serial.print("ODOMETER called updateDistance and distance = ");
		Serial.println(_distance);
	#endif
}

double Odometer::getDistance()
{

	#ifdef ODOMETER
		Serial.print("ODOMETER called getDistance and distance = ");
		Serial.println(_distance);
	#endif
		
  return _distance;
}

Odometer& Odometer::operator= (Odometer &some)
{

	#ifdef ODOMETER
		Serial.print("ODOMETER called operator= and distance = ");
		Serial.print(this->_distance);
		Serial.print(" -> ");
		Serial.println(some._distance);
	#endif

  this->_distance = some._distance;
  return *this;
}