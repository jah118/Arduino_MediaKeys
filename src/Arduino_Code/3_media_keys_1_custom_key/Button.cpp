// Button.cpp
#include "Arduino.h"
#include "Button.h"
#include "Bounce2.h"

Button::Button(byte buttonPin)
  : k_buttonPin(buttonPin),
    k_counter(0),
    k_buttonPressTimeout(750),  // Button press timeout in ms.
    k_previousMillis(0) {
}


void Button::Update() {
  if (k_counter > 0 && millis() - k_previousMillis >= k_buttonPressTimeout) {
    Serial.println("Count from Update() just before it's reset to 0 = ");
    Serial.println(GetCounter());
    k_counter = 0;
  }
}

void Button::IncrementCounter() {
  k_counter++;
  if (k_counter > 4) {
    k_counter = 4;
  }
  if (k_counter == 1) {
    k_previousMillis = millis();
  }
}

// friend void void Button::IncrementCounter(Button &);

void Button::IncrementCounter(Button &) {
  IncrementCounter();
}

byte Button::GetCounter() {
  return k_counter;
}
