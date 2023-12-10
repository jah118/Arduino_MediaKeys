// Button.cpp
#include "Arduino.h"
#include "Button.h"
// #include <Bounce2.h>

// Button::Button(byte buttonPin)
//   : k_buttonPin(buttonPin),
//     k_counter(0),
//     k_buttonPressTimeout(750),  // Button press timeout in ms.
//     k_previousMillis(0) {
// }

// default  where -- Button timing variables
// int debounce = 20;          // ms debounce period to prevent flickering when pressing or releasing the button
// int DCgap = 250;            // max ms between clicks for a double click event
// int holdTime = 1000;        // ms hold period: how long to wait for press+hold event
// int longHoldTime = 3000;    // ms long hold period: how long to wait for press+hold event
Button::Button(byte buttonPin)
  : m_buttonPin(buttonPin),
    m_counter(0),
    m_buttonPressTimeout(750),
    m_previousMillis(0),
    debugState(false),
    debounce(20),
    DCgap(250),
    holdTime(1000),
    longHoldTime(3000),
    buttonVal(HIGH),
    buttonLast(HIGH),
    DCwaiting(false),
    DConUp(false),
    singleOK(true),
    downTime(-1),
    upTime(-1),
    ignoreUp(false),
    waitForUp(false),
    holdEventPast(false),
    longHoldEventPast(false) {
}

Button::Button(byte buttonPin, bool debugState)
  : m_buttonPin(buttonPin),
    m_counter(0),
    m_buttonPressTimeout(750),
    m_previousMillis(0),
    debugState(debugState),
    debounce(20),
    DCgap(250),
    holdTime(1000),
    longHoldTime(3000),
    buttonVal(HIGH),
    buttonLast(HIGH),
    DCwaiting(false),
    DConUp(false),
    singleOK(true),
    downTime(-1),
    upTime(-1),
    ignoreUp(false),
    waitForUp(false),
    holdEventPast(false),
    longHoldEventPast(false) {
}


// Ctor with Custom Button timing variables
Button::Button(byte buttonPin, int debounce, int DCgap, int holdTime, int longHoldTime, bool debugState)
  : m_buttonPin(buttonPin),
    m_counter(0),
    m_buttonPressTimeout(750),
    m_previousMillis(0),
    debugState(debugState),
    debounce(debounce),
    DCgap(DCgap),
    holdTime(holdTime),
    longHoldTime(longHoldTime),
    buttonVal(HIGH),
    buttonLast(HIGH),
    DCwaiting(false),
    DConUp(false),
    singleOK(true),
    downTime(-1),
    upTime(-1),
    ignoreUp(false),
    waitForUp(false),
    holdEventPast(false),
    longHoldEventPast(false) {
}

void Button::Update() {
  if (m_counter > 0 && millis() - m_previousMillis >= m_buttonPressTimeout) {
    Serial.print("Count from Update() just before it's reset to 0 = ");
    Serial.println(GetCounter());
    m_counter = 0;
  }
}


void Button::IncrementCounter() {
  m_counter++;
  if (m_counter > 4) {
    m_counter = 4;
  }
  if (m_counter == 1) {
    m_previousMillis = millis();
  }
}

// friend void void Button::IncrementCounter(Button &);

void Button::IncrementCounter(Button &) {
  IncrementCounter();
}

byte Button::GetCounter() {
  return m_counter;
}

void Button::isDebugTruePrintToSerial(String temp) {
  if (debugState) {
    Serial.println(temp);
  }
}

int Button::CheckButtonEvent() {
  // isDebugTruePrintToSerial("CheckButtonEvent start");
  int event = 0;
  buttonVal = digitalRead(m_buttonPin);
  // isDebugTruePrintToSerial("CheckButtonEvent  " + String(buttonVal));

  // Button pressed down
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > debounce) {
    isDebugTruePrintToSerial("CheckButtonEvent  pressed " + String(buttonVal));
    downTime = millis();
    ignoreUp = false;
    waitForUp = false;
    singleOK = true;
    holdEventPast = false;
    longHoldEventPast = false;
    if ((millis() - upTime) < DCgap && DConUp == false && DCwaiting == true) DConUp = true;
    else DConUp = false;
    DCwaiting = false;
  }
  // Button released
  else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce) {
    isDebugTruePrintToSerial("CheckButtonEvent  released ");

    if (not ignoreUp) {
      upTime = millis();
      if (DConUp == false) DCwaiting = true;
      else {
        event = 2;
        DConUp = false;
        DCwaiting = false;
        singleOK = false;
        isDebugTruePrintToSerial("CheckButtonEvent Event: released event 2");
      }
    }
    isDebugTruePrintToSerial("CheckButtonEvent Event:" + String(event));
  }
  // Test for normal click event: DCgap expired
  if (buttonVal == HIGH && (millis() - upTime) >= DCgap && DCwaiting == true && DConUp == false && singleOK == true && event != 2) {
    event = 1;
    DCwaiting = false;
    isDebugTruePrintToSerial("CheckButtonEvent Event: normal");
  }
  // Test for hold
  if (buttonVal == LOW && (millis() - downTime) >= holdTime) {
    // Trigger "normal" hold
    if (not holdEventPast) {
      event = 3;
      waitForUp = true;
      ignoreUp = true;
      DConUp = false;
      DCwaiting = false;
      //downTime = millis();
      holdEventPast = true;
    }
    // Trigger "long" hold
    if ((millis() - downTime) >= longHoldTime) {
      if (not longHoldEventPast) {
        event = 4;
        longHoldEventPast = true;
      }
    }
    isDebugTruePrintToSerial("CheckButtonEvent Event:" + String(event));
  }
  buttonLast = buttonVal;



  return event;
}
