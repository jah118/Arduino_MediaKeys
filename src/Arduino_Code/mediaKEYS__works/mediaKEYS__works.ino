/*
  Jonas

  Arduino Media Keys
  Version 0.7
  03/04/2021

  Dependencies:
  HID-Project by NicoHood Version 2.5.0 or above
  - can be found in Arduino Lirary Manager
  - or: https://github.com/NicoHood/HID

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
*/


//      TODO FOR V2 
/*
    use #include <Bounce2.h> for better douldbe press 
    https://arduino.stackexchange.com/questions/69897/detecting-how-many-button-pressessingle-tap-double-tap-etc-with-two-simultane

    ensure / add support for auto change beween linux and windows. 

    V3 
    change over to load key map file - maybejson 
    read json AND  read it from pc program or some nice wayy that needs no reflash 
    https://forum.arduino.cc/t/writing-to-flash-from-application-please-test-and-enjoy/320295

*/

//test singel or double press of keyy

// include the HID library
#include "HID-Project.h"
#include "FastLED.h" 
#include "Bounce2.h"

//look up hid projekt to see Languages  or see AlternateLanguageLayout.ino from hid project for example
#define HID_CUSTOM_LAYOUT // set this flag to indicate that a custom layout is selected
#define LAYOUT_DANISH     // set this flag after the above flag to indicate the custom input method is DANISH

//rgb
#define DATA_PIN 9
//#define CLK_PIN 4
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 8
#define BRIGHTNESS 32

CRGB leds[NUM_LEDS];

int rgbState = 1;
int lastrgbState = 0;

//press const for determening a press
unsigned long onTime;
int lastReading = LOW;
int bounceTime = 35; //50
int holdTime = 500;  //500
unsigned int hold = 0;

long lastSwitchTime = 0;
long doubleTime = 150; // 150 //230


int reading;

const int backButton = 4; // key 1
const int playButton = 5; // key 2
const int fwdButton = 6;  // key 3
const int customButton = 7;

const int delayConst75 = 75;
const int delayConst250 = 250;

const bool debugState = true;

void isDebugTruePrintToSerial(String temp)
{
  if (debugState)
  {
    Serial.println(temp);
  }
}

// n = led
void turnOnRGBByState(int cRed, int cGreen, int cBlue, bool state, int n)
{
  switch (state)
  {
  case 0:
    FastLED.clear();
    leds[n] = CRGB(cRed, cGreen, cBlue);
    for (int i = 0; i <= NUM_LEDS - 1; i++)
    {
      leds[n + i] = CRGB(cRed, cGreen, cBlue);
    }

    isDebugTruePrintToSerial(state + "");
    isDebugTruePrintToSerial(lastrgbState + "");
    FastLED.show();
    lastrgbState = 0;
    rgbState = 0;
    break;
  case 1:
    isDebugTruePrintToSerial("red");
    FastLED.clear();
    isDebugTruePrintToSerial("lastrgbState black");
    isDebugTruePrintToSerial(lastrgbState + "");
    leds[2] = CRGB::Black;
    FastLED.show();
    lastrgbState = 0;
    rgbState = 0;
    break;
  }
}

void press()
{
  if ((millis() - lastSwitchTime) < doubleTime)
  {
    isDebugTruePrintToSerial("double press");

    Keyboard.write(KEY_F16);
    turnOnRGBByState(0, 0, 250, lastrgbState, 0);
    //    Keyboard.println("You pressed the button ");
  }
  else if ((millis() - lastSwitchTime) > doubleTime)
  {
    isDebugTruePrintToSerial("single press");
    Keyboard.write(KEY_F19);
    switch (rgbState)
    {
    case 0:
      isDebugTruePrintToSerial("case 0");
      isDebugTruePrintToSerial("last rgbState: " + lastrgbState);
      isDebugTruePrintToSerial("rgbState black : " + rgbState);
      turnOnRGBByState(0, 0, 0, lastrgbState, 0);
      rgbState = 1;
      break;
    case 1:
      isDebugTruePrintToSerial("case 1");
      turnOnRGBByState(255, 0, 0, lastrgbState, 0);
      rgbState = 0;
      isDebugTruePrintToSerial("new val rgbState red");
      isDebugTruePrintToSerial(rgbState + "");
      break;
    }
  }
  lastSwitchTime = millis();
}


void setup()
{
  Serial.begin(115200);
  delay(20);

  // define the pin mode for each pin used
  pinMode(playButton, INPUT_PULLUP);
  pinMode(fwdButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);

  //press decllare
  pinMode(customButton, INPUT_PULLUP);

  // begin HID connection
  //  BootKeyboard.begin(); //old way useful for bios and or logout
  Keyboard.begin();
  Consumer.begin();

  delay(3000); // 3 second delay for recovery
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  for (int dot = 0; dot < NUM_LEDS - 1; dot++)
  {
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(30);
  }

  reading = digitalRead(customButton);
}

void loop()
{

  //single or double  pressed
  //-----------------------------------------------------------------------------------------
  reading = digitalRead(customButton);
  if (reading == HIGH && lastReading == LOW)
  {
    onTime = millis();
  }
  else if (reading == LOW && lastReading == HIGH)
  {
    //    onTime = millis();
    if (((millis() - onTime) > bounceTime) && hold != 1)
    {
      press();
    }
    if (hold = 1)
    {
      hold = 0;
    }
  }

  lastReading = reading;
  //__________________________________________________________________________________________
  // all the buttons follow the same pattern ...

  // if the play button is pressed
  if (!digitalRead(playButton))
  {
    Consumer.write(MEDIA_PLAY_PAUSE); // send HID command
    isDebugTruePrintToSerial("Play/Pause");
    delay(delayConst250); // wait
  }

  if (!digitalRead(fwdButton))
  {
    Consumer.write(MEDIA_NEXT);
    isDebugTruePrintToSerial("Next");
    delay(delayConst250);
  }

  if (!digitalRead(backButton))
  {
    Consumer.write(MEDIA_PREVIOUS);
    isDebugTruePrintToSerial("Back");
    delay(delayConst250);
  }
}
