#include "Bounce2.h"
#include "Button.h"

/*
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
    */

    void Update()
    {
      if (k_counter > 0 && millis() - k_previousMillis >= k_buttonPressTimeout)
      {
        isDebugTruePrintToSerial("Count from Update() just before it's reset to 0 = ");
        Serial.println(GetCounter());
       //  Serial.println(char(GetCounter()));                  <-----------------------------_____--------------------- will this work ?? 
        k_counter = 0;
      }
    }

    void IncrementCounter()
    {
      k_counter++;
      if (k_counter > 4)
      {
        k_counter = 4;
      }
      if (k_counter == 1)
      {
        k_previousMillis = millis();
      }
    }

    friend void IncrementCounter(Button &);

    void IncrementCounter(Button &)
    {
      IncrementCounter();
    }

    byte GetCounter()
    {
      return k_counter;
    }