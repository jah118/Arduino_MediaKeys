// Button.cpp
#include "Arduino.h"
#include "Button.h"

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

// CheckButtonEvent Function
// This function checks the state of the button and determines the type of button event.
// It supports Click, Double-Click, Hold, and Long Hold based on the 4-Way Button concept.
//  It returns  Click = 1, Double-Click = 2, Hold = 3, Long Hold = 4
//
//  4-Way Button: Cli ck, Double-Click, Hold, Long Hold - Based on <a href="https://jmsarduino.blogspot.com/2009/10/4-way-button-click-double-click-hold.html ">4-Way Button</a>
int Button::CheckButtonEvent() {
  int event = 0;
  buttonVal = digitalRead(m_buttonPin);

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
