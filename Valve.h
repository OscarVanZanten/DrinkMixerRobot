#ifndef Valve_h
#define Valve_h

#include "Arduino.h"

class Valve {
  private:
    byte pin;
    short timing;
    bool enabled;

  public:
    Valve();
    Valve(byte pin, short timing);
    void Activate();
};

Valve::Valve()
{
  this->enabled = false;
}

Valve::Valve(byte pin, short timing)
{
  pinMode(pin, OUTPUT);
  this->pin = pin;
  this->timing = timing;
  this->enabled = true;
}

void Valve::Activate()
{
  if (this->enabled) {
    digitalWrite(this->pin, HIGH);
    delay(this->timing);
    digitalWrite(this->pin, LOW);
  }
}

#endif
