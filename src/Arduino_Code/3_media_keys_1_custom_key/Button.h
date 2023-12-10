// Button.h
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "Bounce2.h"

class Button {
private:
  byte k_buttonPin;
  byte k_counter = 0;
  unsigned long k_buttonPressTimeout;
  unsigned long k_previousMillis;

public:
  // OLD -- needs more testing  deleteing old code
  // Button(byte buttonPin) : k_buttonPin(buttonPin),
  //                          k_counter(0),
  //                          k_buttonPressTimeout(750), // Button press timeout in ms.
  //                          k_previousMillis(0)
  // {
  // }

  // NEW -- 
  Button(byte buttonPin);

  void Update();

  void IncrementCounter();
  friend void IncrementCounter(Button &);

  void IncrementCounter(Button &);
  byte GetCounter();
};
#endif
