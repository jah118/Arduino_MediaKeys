#include "Bounce2.h"
#include <Arduino.h>

class Button
{
  private:
    byte k_buttonPin;
    byte k_counter = 0;
    unsigned long k_buttonPressTimeout;
    unsigned long k_previousMillis;

  public:
    Button(byte buttonPin) : k_buttonPin(buttonPin),
      k_counter(0),
      k_buttonPressTimeout(750), // Button press timeout in ms.
      k_previousMillis(0)
    {
    }

    void Update();

    void IncrementCounter();
    friend void IncrementCounter(Button &);

    void IncrementCounter(Button &);
    byte GetCounter();
};
