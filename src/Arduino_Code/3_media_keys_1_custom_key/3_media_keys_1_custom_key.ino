/*
  Jonas

  Arduino Media Keys
  Version V0.8.1
  10/12/2023

  Dependencies:
  - HID-Project by NicoHood Version 2.5.0 or above
    -- can be found in Arduino Lirary Manager
    -- or: https://github.com/NicoHood/HID
  
  - FastLED
  - Bounce2

  Reference:
  The Reference links below is where the info to getting the project to work comes from
  https://uboopenfactory.univ-brest.fr/Les-Labs/MusicLab/Projets/Arduino-Media-Keys
  - most code comes from here.
  https://github.com/NicoHood/HID/wiki
  https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
  https://www.stefanjones.ca/blog/arduino-leonardo-remote-multimedia-keys/
  https://arduino.stackexchange.com/questions/8934/send-keyboard-media-keys-with-keyboard-library
  http://www.freebsddiary.org/APC/usb_hid_usages.php
  - list of hid codes for further use or differt use case
  https://www.instructables.com/id/USB-Volume-Control-and-Caps-Lock-LED-Simple-Cheap-/
  https://docs.arduino.cc/hacking/software/LibraryTutorial

  double press solutin based on:
  MULTI-CLICK:  One Button, Multiple Events Based on "https://jmsarduino.blogspot.com/2009/10/4-way-button-click-double-click-hold.html"

*/


/*
  //      V0.8 - done
    use #include <Bounce2.h> for better double  press
    https://arduino.stackexchange.com/questions/69897/detecting-how-many-button-pressessingle-tap-double-tap-etc-with-two-simultane

    ensure / add support for auto change beween linux and windows.
    //      V0.8.1 - done
    * Replaced Bounce2.h as  double  press handler using :
      MULTI-CLICK:  One Button, Multiple Events Based on "https://jmsarduino.blogspot.com/2009/10/4-way-button-click-double-click-hold.html"

    *  Moved  Button code to lib : Button.h
    * started on V0.9.0 base classes for loading confs

  //        TODO FOR V0.9.0
    change over to load key map file - maybejson
    read json AND  read it from pc program or some nice wayy that needs no reflash :
    https://forum.arduino.cc/t/writing-to-flash-from-application-please-test-and-enjoy/320295
    https://arduinoplusplus.wordpress.com/2019/04/02/persisting-application-parameters-in-eeprom/
    https://arduinojson.org/v6/how-to/store-a-json-document-in-eeprom/
    https://arduinojson.org/
    https://arduinojson.org/v6/how-to/reduce-memory-usage/

  // TODO goals for v1.0.0
    add serial support between arduione and esp to send keypress or recives keypress
      - reason this is to add support for esp or raspberry zero with screen that shows media status.
      - if zero is used then a webpage can be used. show and pull data from there. do browser in terminal to save resources, and make it host is own webpages that is displayed
      - if esp is used thne look in to building ui that where some part is static and and only progress bar moves https://forum.arduino.cc/t/tft-touch-reaching-high-refresh-rate-cost-effectively/595823/6
        and rest of sreen updates only on new song changed (this not for first version, first version shows the name of the song played  )
        https://www.youtube.com/watch?v=rq5yPJbX_uk
        https://www.esp8266.com/viewtopic.php?f=32&t=16366


*/


// look up hid projekt to see Languages  or see AlternateLanguageLayout.ino from hid project for example
#define HID_CUSTOM_LAYOUT  // set this flag to indicate that a custom layout is selected
#define LAYOUT_DANISH      // set this flag after the above flag to indicate the custom input method is DANISH

// include the HID library
#include "HID-Project.h"
#include "FastLED.h"
#include "Bounce2.h"

// TODO add defin things to struct and load from json



//------------------------------ rgb ------------------------------
#define DATA_PIN 9
//#define CLK_PIN 4
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 8
#define BRIGHTNESS 32

CRGB leds[NUM_LEDS];  // TODO maybe move beacause of loading order of data and info from config

int rgbState = 1;
int lastrgbState = 0;

// ------------------------------ Buttons press vars ------------------------------
// press var's for determening a press or double press
int holdTime = 1000;

unsigned long lastSwitchTime = 0;
long DCgap = 250;   // 150 //230   // time for captureing a double press
int debounce = 20;  // 50 //20 // 35

// Constants
const byte BUTTON_DEBOUNCE_INTERVAL = 25;

const byte buttonPin1 = 4;  // key 1 backButton
const byte buttonPin2 = 5;  // key 2 playButton
const byte buttonPin3 = 6;  // key 3 fwdButton
const byte buttonPin4 = 7;  // key 4 customButton F16/F19

const uint8_t buttonPin1Action = MEDIA_PREVIOUS;
const uint8_t buttonPin2Action = MEDIA_PLAY_PAUSE;
const uint8_t buttonPin3Action = MEDIA_NEXT;
const uint8_t buttonPin4Action1 = KEY_F19;
const uint8_t buttonPin4Action2 = KEY_F16;

// ------------------------------ DEBUG ------------------------------
// Determs if Serial.print is enabled
const bool debugState = true;

void isDebugTruePrintToSerial(String temp) {
  if (debugState) {
    Serial.println(temp);
  }
}

// ------------------------------ Dataconf ------------------------------

// Struct to hold the general config like colours.
struct Config {
  char layout;
  uint16_t datapin;
  char ledtype;
  char colororder;
  uint16_t nleds;
  uint16_t brightness;
  uint8_t debounce;  // ms debounce period to prevent flickering when pressing or releasing the button
  uint8_t holdTime;  // ms hold period: how long to wait for press+hold event
  uint8_t DCgap;     // max ms between clicks for a double click event
  uint8_t attempts;
  uint16_t attemptdelay;
};

//--------- Internal references ------------
// (this needs to be below all structs etc..) this may needed to be loaded after struct
// #include "configLoad.h"
#include "Button.h"

// -------------- Start Button  ----------------------
// Bounce2.h lib setup
Bounce button1Debouncer = Bounce();
Bounce button2Debouncer = Bounce();
Bounce button3Debouncer = Bounce();

// Button.h lib setup
Button Button1(buttonPin1, debugState);
Button Button2(buttonPin2, debugState);
Button Button3(buttonPin3, debugState);
Button Button4(buttonPin4, debugState);

// -------------- Start filesystem ----------------------

// if (!FILESYSTEM.begin())
//{
//   Serial.println("[ERROR]: SPIFFS initialisation failed!");
//   drawErrorMessage("Failed to init SPIFFS! Did you upload the data folder?");
//   while (1)
//     yield(); // We stop here
// }
// Serial.println("[INFO]: SPIFFS initialised.");
//
//// Check for free space
//
// Serial.print("[INFO]: Free Space: ");
// Serial.println(SPIFFS.totalBytes() - SPIFFS.usedBytes());

//------------------ Load Config ----------------------------------------------

// Serial.println("[INFO]: Loading Config");
// if (!configLoad())
//{
//   Serial.println("[WARNING]: Failed to load config!");
// }
// else
//{
//   Serial.println("[INFO]: Config Loaded");
// }

//------------------ Functions ----------------------------------------------


// TODO
// n = how many leds need be turned on, is used as a array this enables to not turn all the leds.
// state = used for turning the leds on and of as a toggle
void turnOnRGBByState(int cRed, int cGreen, int cBlue, bool state, int n) {
  switch (state) {
    case 0:
      FastLED.clear();
      leds[n] = CRGB(cRed, cGreen, cBlue);
      for (int i = 0; i <= NUM_LEDS - 1; i++) {
        leds[n + i] = CRGB(cRed, cGreen, cBlue);
      }

      isDebugTruePrintToSerial(state + "");
      isDebugTruePrintToSerial(lastrgbState + "");
      FastLED.show();
      lastrgbState = 0;
      rgbState = 0;
      break;
    case 1:  // is used for turning it off
      FastLED.clear();
      isDebugTruePrintToSerial("lastrgbState black");
      isDebugTruePrintToSerial(lastrgbState + "");
      leds[NUM_LEDS] = CRGB::Black;
      FastLED.show();
      lastrgbState = 0;
      rgbState = 0;
      break;
  }
}

void pressEventHandler(int event) {
  switch (event) {
    case 1:
      clickEvent();
      isDebugTruePrintToSerial("single press");
      Keyboard.write(KEY_F19);
      switch (rgbState) {
        case 0:
          isDebugTruePrintToSerial("case 0");
          turnOnRGBByState(0, 0, 0, lastrgbState, 0);
          rgbState = 1;
          break;
        case 1:
          isDebugTruePrintToSerial("case 1");
          turnOnRGBByState(255, 0, 0, lastrgbState, 0);
          rgbState = 0;
          break;
      }
      break;
    case 2:
      doublePress(buttonPin4Action2);
      doubleClickEvent();
      break;
    case 3:
      holdEvent();
      break;
    case 4:
      longHoldEvent();
      break;
    default:
      break;
  }
}


void doublePress(KeyboardKeycode key) {
  // Double press
  isDebugTruePrintToSerial("double press");
  // Action to be performed on double press
  Keyboard.write(key);
  turnOnRGBByState(0, 0, 250, lastrgbState, 0);
}

// Initializes buttons and debouncers for setup.
void setupButtons() {
  // Define the pin mode for each pin used
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);

  // BUTTON Setup for Bounce2
  button1Debouncer.attach(buttonPin1);
  button2Debouncer.attach(buttonPin2);
  button3Debouncer.attach(buttonPin3);

  // DEBOUNCE INTERVAL IN MILLISECONDS
  button1Debouncer.interval(BUTTON_DEBOUNCE_INTERVAL);
  button2Debouncer.interval(BUTTON_DEBOUNCE_INTERVAL);
  button3Debouncer.interval(BUTTON_DEBOUNCE_INTERVAL);
}

void setup() {
  Serial.begin(115200);
  delay(20);  // Delay for recovery

  setupButtons();  // encapsulated buttons and debouncers basic setup

  // Begin HID connection
  Keyboard.begin();
  Consumer.begin();

  delay(1500);  // 1.5 second delay for recovery and HID connection is sometime unstable at start, this limits it to less.

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);


  FastLED.setBrightness(BRIGHTNESS);  // set master brightness control
  for (int dot = 0; dot < NUM_LEDS - 1; dot++) {
    FastLED.show();
    leds[dot] = CRGB::Black;  // clear this led for the next time around the loop
    delay(30);
  }
}

void loop() {
  // Calls the Update function as fast as possible.
  Button1.Update();
  Button2.Update();
  Button3.Update();
  Button4.Update();

  // Button 1 pressed.
  if (button1Debouncer.update()) {
    if (button1Debouncer.fell())  // key is pressed down
    {
      Consumer.write(buttonPin1Action);  // send HID command
      isDebugTruePrintToSerial("Back");
      // Check if button 1 and 2 pressed together
      if (digitalRead(buttonPin2) == 0) {
        Button1.IncrementCounter();
      }
    }
  }

  // Button 2 pressed.
  if (button2Debouncer.update()) {
    if (button2Debouncer.fell())  // key is pressed down
    {
      Consumer.write(buttonPin2Action);  // send HID command
      isDebugTruePrintToSerial("Play/Pause");
      // Check if button 1 and 2 pressed together
      if (digitalRead(buttonPin1) == 0) {
        Button1.IncrementCounter(Button2);
      }
    }
  }

  // Button 3 pressed.
  if (button3Debouncer.update()) {
    if (button3Debouncer.fell())  // key is pressed down
    {
      // Check if button 3 and 4 pressed together
      if (digitalRead(buttonPin4) == 0) {
        Button3.IncrementCounter();
      } else {
        Consumer.write(buttonPin3Action);  // send HID command
        isDebugTruePrintToSerial("Next");
      }
    }
  }

  int b = Button4.CheckButtonEvent();
  pressEventHandler(b);  // this is only for button 4
}

// =================================================  TODO this part of prep for v0.9.0
// Events to trigger -- TODO this has to be generict so configlod fits into these events where a button returns event int and then conf can be read an executed

void clickEvent() {
  isDebugTruePrintToSerial("clickEvent");
}
void doubleClickEvent() {
  isDebugTruePrintToSerial("doubleClickEvent");
}
void holdEvent() {
  isDebugTruePrintToSerial("holdEvent");
}
void longHoldEvent() {
  isDebugTruePrintToSerial("longHoldEvent");
}
