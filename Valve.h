#ifndef Valve_h
#define Valve_h

#include "Arduino.h"

class Valve {
  private:
    byte pin;
    short timing;
 
  public:
    Valve(byte pin, short timing);
    void Activate();
};

Valve::Valve(byte pin, short timing)
{
 pinMode(pin, OUTPUT);
 this->pin = pin;
 this->timing = timing;
}

void Valve::Activate()
{
  digitalWrite(pin, HIGH);
  delay(timing);
  digitalWrite(pin, LOW);
}

#endif
