// Button.h
/*
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

class Button {
private:
  byte m_buttonPin;
  byte m_counter;
  unsigned long m_buttonPressTimeout;
  unsigned long m_previousMillis;
  bool debugState;  // Determs if Serial.print is enabled

  // =================================================
  //  MULTI-CLICK:  One Button, Multiple Events Based on <a href="https://jmsarduino.blogspot.com/2009/10/4-way-button-click-double-click-hold.html">4-Way Button</a>

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
  Button(byte buttonPin);
  Button(byte buttonPin, bool debugState);
  Button(byte buttonPin, int debounce, int DCgap, int holdTime, int longHoldTime, bool debugState);

  void Update();
  void IncrementCounter();
  // friend void IncrementCounter(Button &);
  void IncrementCounter(Button &);
  byte GetCounter();
  int CheckButtonEvent();
  void isDebugTruePrintToSerial(String temp);
};
#endif
