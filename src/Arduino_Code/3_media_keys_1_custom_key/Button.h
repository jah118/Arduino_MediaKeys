// Button.h
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
// #include <Bounce2.h>

class Button {
private:
  byte m_buttonPin;
  byte m_counter;
  unsigned long m_buttonPressTimeout;
  unsigned long m_previousMillis;
  // Determs if Serial.print is enabled
  bool debugState;

  // ---temp test
  // // Button timing variables
  // int debounce = 20;        // ms debounce period to prevent flickering when pressing or releasing the button
  // int DCgap = 250;          // max ms between clicks for a double click event
  // int holdTime = 1000;      // ms hold period: how long to wait for press+hold event
  // int longHoldTime = 3000;  // ms long hold period: how long to wait for press+hold event

  // // Button variables
  // boolean buttonVal = HIGH;           // value read from button
  // boolean buttonLast = HIGH;          // buffered value of the button's previous state
  // boolean DCwaiting = false;          // whether we're waiting for a double click (down)
  // boolean DConUp = false;             // whether to register a double click on next release, or whether to wait and click
  // boolean singleOK = true;            // whether it's OK to do a single click
  // long downTime = -1;                 // time the button was pressed down
  // long upTime = -1;                   // time the button was released
  // boolean ignoreUp = false;           // whether to ignore the button release because the click+hold was triggered
  // boolean waitForUp = false;          // when held, whether to wait for the up event
  // boolean holdEventPast = false;      // whether or not the hold event happened already
  // boolean longHoldEventPast = false;  // whether or not the long hold event happened already

  // Button timing variables
  int debounce;      // ms debounce period to prevent flickering when pressing or releasing the button
  int DCgap;         // max ms between clicks for a double click event
  int holdTime;      // ms hold period: how long to wait for press+hold event
  int longHoldTime;  // ms long hold period: how long to wait for press+hold event

  // Button variables
  bool buttonVal;          // value read from button
  bool buttonLast;         // buffered value of the button's previous state
  bool DCwaiting;          // whether we're waiting for a double click (down)
  bool DConUp;             // whether to register a double click on next release, or whether to wait and click
  bool singleOK;           // whether it's OK to do a single click
  long downTime;           // time the button was pressed down
  long upTime;             // time the button was released
  bool ignoreUp;           // whether to ignore the button release because the click+hold was triggered
  bool waitForUp;          // when held, whether to wait for the up event
  bool holdEventPast;      // whether or not the hold event happened already
  bool longHoldEventPast;  // whether or not the long hold event happened already



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
  Button(byte buttonPin, bool debugState);

  Button(byte buttonPin, int debounce, int DCgap, int holdTime, int longHoldTime, bool debugState);
  void Update();
  void IncrementCounter();
  friend void IncrementCounter(Button &);
  void IncrementCounter(Button &);
  byte GetCounter();
  int CheckButtonEvent();
  void isDebugTruePrintToSerial(String temp);
};
#endif
