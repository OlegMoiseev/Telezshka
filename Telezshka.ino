#include <Array.h>

Array<double, 6> xyz;

const int opto1 = 29;
const int motorRollSpd1 = 5;
const int motorRollReverse1 = 25;

const int opto2 = 28;
const int motorRollSpd2 = 6;
const int motorRollReverse2 = 26;

class wheel
{
    const int optoPin;
    const int motorRollSpdPin;
    const int motorRollReversePin;
    
    double sendSpd;
    double desiredSpd;
    double currentSpd;
    
    bool prevState;
    unsigned long prevTime;
    
    /*TODO: Realize it!
      int stopMove();
    */
    
    int rollForward();
    int rollBackward();
    
    void updateCurRollSpd();
    
  public:
    wheel(int opto, int mRSP, int mRR);
    int setRollSpd(double spdIn);
    void updateRollSpd();
    void stopMove();
};

wheel::wheel(int opto, int mRS, int mRR)
  :
  optoPin(opto),
  motorRollSpdPin(mRS),
  motorRollReversePin(mRR),
  prevTime(micros()),
  sendSpd(0),
  currentSpd(0),
  desiredSpd(0)
{
  pinMode(optoPin, INPUT);
  pinMode(motorRollSpdPin, OUTPUT);
  pinMode(motorRollReversePin, OUTPUT);
  
  prevState = digitalRead(optoPin);
}

int wheel::rollBackward()
{
  digitalWrite(motorRollReversePin, HIGH);
}

int wheel::rollForward()
{
  digitalWrite(motorRollReversePin, LOW);
}

void wheel::updateCurRollSpd()
{
  bool curState = digitalRead(optoPin);
  if (curState != prevState)
  {
    unsigned long stepTime = micros() - prevTime;
    
    prevState = curState;
    prevTime = micros();
    curState == true ? stepTime *= 1.382239382 : stepTime *= 0.7306122449;
    static_cast<double>(stepTime);
    currentSpd = 5. / (stepTime / 1e6);  // 5 mm / (time in microseconds / 1 000 000);
  }
}

int wheel::setRollSpd(double spdIn)
{
  desiredSpd = spdIn;
}

void wheel::stopMove()
{
  analogWrite(motorRollSpdPin, 0);
}



void wheel::updateRollSpd()
{
  if (abs(desiredSpd) < 1.0)
  {
    stopMove();
  }
  else
  {
    updateCurRollSpd();
    desiredSpd > 0 ? rollForward() : rollBackward();
    double delta = abs(desiredSpd) - currentSpd;
    if (abs(delta) > 10.0)
    {
      sendSpd += (255.0 / 200.0) * delta / 500.0;  // 200 - maxSpeed 2.0 - koef PID 
      if (sendSpd > 255)
      {
        sendSpd = 255;
      }
      if (sendSpd < 0)
      {
        sendSpd = 50;
      }
      analogWrite(motorRollSpdPin, sendSpd);
    }
  }
  /*Serial.print("Current speed:\t");
  Serial.print(currentSpd);
  Serial.print("\tDesired speed:\t");
  Serial.print(desiredSpd);
  Serial.print("\tDelta:\t");
  Serial.print(delta);
  Serial.print("\tSend speed:\t");
  Serial.println(sendSpd);*/
  //Serial.println(sendSpd);
}

wheel wheel1(opto1, motorRollSpd1, motorRollReverse1);
wheel wheel2(opto2, motorRollSpd2, motorRollReverse2);

void setup()
{
  Serial.begin(9600);
  Serial.println("Started!");
}

//180.0 0.0 0.0 0.0 0.0 0.0
//180.0 0.0 180.0 0.0 180.0 0.0
//205.0 0.0 125.0 0.0 65.0 0.0

// 0.0 50.0 0.0 0.0 0.0 0.0
void loop()
{
  if (Serial.available() > 0)
  {
    for (int i = 0; i < 6; ++i)
    {
      xyz.at(i) = Serial.parseFloat();
      Serial.println(xyz.at(i));
    }
    wheel1.setRollSpd(xyz.at(1));
    wheel2.setRollSpd(xyz.at(3));
  }  
  wheel1.updateRollSpd();
  wheel2.updateRollSpd();
}

