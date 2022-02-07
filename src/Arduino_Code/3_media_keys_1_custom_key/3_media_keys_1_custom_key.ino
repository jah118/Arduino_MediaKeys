/*
  Jonas

  Arduino Media Keys
  Version 0.8
  06/02/2022

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

  //        TODO FOR V3
    change over to load key map file - maybejson
    read json AND  read it from pc program or some nice wayy that needs no reflash
    https://forum.arduino.cc/t/writing-to-flash-from-application-please-test-and-enjoy/320295

*/
/*'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' Notes / bugs  *''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // test singel or double press of keyy
  // if arduino is on for about 50 days mills() will overflow. The var's 'ontime' and 'lastSwitchTime'
      -- source https://www.arduino.cc/reference/en/language/functions/time/millis/

  ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''*''''''''*''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''*/

// include the HID library
#include "HID-Project.h"
#include "FastLED.h"
#include "Bounce2.h"

// look up hid projekt to see Languages  or see AlternateLanguageLayout.ino from hid project for example
#define HID_CUSTOM_LAYOUT // set this flag to indicate that a custom layout is selected
#define LAYOUT_DANISH     // set this flag after the above flag to indicate the custom input method is DANISH

// rgb
#define DATA_PIN 9
//#define CLK_PIN 4
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 8
#define BRIGHTNESS 32

CRGB leds[NUM_LEDS];

int rgbState = 1;
int lastrgbState = 0;

// press var's for determening a press or double press
unsigned long onTime;
int lastReading = LOW;
int bounceTime = 35;      // 50
int holdTime = 500;       // 500
unsigned int hold = 0;

unsigned long lastSwitchTime = 0;
long doubleTime = 230;    // 150 //230

int reading;

const int buttonPin1 = 4; // key 1 backButton
const int buttonPin2 = 5; // key 2 playButton
const int buttonPin3 = 6; // key 3 fwdButton
const int buttonPin4 = 7; // key 4 customButton F16/F19

const int delayConst75 = 75;

// Determs if Serial.print is enabled
const bool debugState = true;

void isDebugTruePrintToSerial(String temp)
{
  if (debugState)
  {
    Serial.println(temp);
  }
}

// This could be move to its own lib / file  to keep it clean
class Button
{

  private:
    byte m_buttonPin;
    byte m_counter = 0;
    unsigned long m_buttonPressTimeout;
    unsigned long m_previousMillis;

  public:
    Button(byte buttonPin) : m_buttonPin(buttonPin),
      m_counter(0),
      m_buttonPressTimeout(750), // Button press timeout in ms.
      m_previousMillis(0)
    {
    }

    void Update()
    {
      if (m_counter > 0 && millis() - m_previousMillis >= m_buttonPressTimeout)
      {
        isDebugTruePrintToSerial("Count from Update() just before it's reset to 0 = ");
        Serial.println(GetCounter());
        m_counter = 0;
      }
    }

    void IncrementCounter()
    {
      m_counter++;
      if (m_counter > 4)
      {
        m_counter = 4;
      }
      if (m_counter == 1)
      {
        m_previousMillis = millis();
      }
    }

    friend void IncrementCounter(Button &);

    void IncrementCounter(Button &)
    {
      IncrementCounter();
    }

    byte GetCounter()
    {
      return m_counter;
    }
};

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
// TODO optimise this
void press()
{
  if ((millis() - lastSwitchTime) < doubleTime)
  {
    isDebugTruePrintToSerial("double press");

    Keyboard.write(KEY_F16);
    turnOnRGBByState(0, 0, 250, lastrgbState, 0);
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

Bounce button1Debouncer = Bounce();
Bounce button2Debouncer = Bounce();
Bounce button3Debouncer = Bounce();
Bounce button4Debouncer = Bounce();
Button Button1(buttonPin1);
Button Button2(buttonPin2);
Button Button3(buttonPin3);
Button Button4(buttonPin4);

void setup()
{
  Serial.begin(115200);
  delay(20);

  // Define the pin mode for each pin used
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  
  // BUTTON Setup for Bounce2
  button1Debouncer.attach(buttonPin1);
  button2Debouncer.attach(buttonPin2);
  button3Debouncer.attach(buttonPin3);
  button4Debouncer.attach(buttonPin4);
  
  // DEBOUNCE INTERVAL IN MILLISECONDS
  button1Debouncer.interval(25);
  button2Debouncer.interval(25);
  button3Debouncer.interval(25);
  button4Debouncer.interval(25);

  // Begin HID connection
  Keyboard.begin();
  Consumer.begin();

  delay(1500); // 1.5 second delay for recovery
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

  reading = digitalRead(buttonPin4);
}

void loop()
{
  // Calls the Update function as fast as possible.
  Button1.Update();
  Button2.Update();
  Button3.Update();
  Button4.Update();

  // Button 1 pressed.
  if (button1Debouncer.update())
  {
    if (button1Debouncer.fell()) // key is pressed down
    {
      Consumer.write(MEDIA_PREVIOUS); // send HID command
      isDebugTruePrintToSerial("Back");
      // Check if button 1 and 2 pressed together
      if (digitalRead(buttonPin2) == 0)
      {
        Button1.IncrementCounter();
      }
    }
  }

  // Button 2 pressed.
  if (button2Debouncer.update())
  {
    if (button2Debouncer.fell()) // key is pressed down
    {
      Consumer.write(MEDIA_PLAY_PAUSE); // send HID command
      isDebugTruePrintToSerial("Play/Pause");
      // Check if button 1 and 2 pressed together
      if (digitalRead(buttonPin1) == 0)
      {
        Button1.IncrementCounter(Button2);
      }
    }
  }

  // Button 3 pressed.
  if (button3Debouncer.update())
  {
    if (button3Debouncer.fell()) // key is pressed down
    {
      // Check if button 3 and 4 pressed together
      if (digitalRead(buttonPin4) == 0)
      {
        Button3.IncrementCounter();
      }
      else
      {
        Consumer.write(MEDIA_NEXT); // send HID command
        isDebugTruePrintToSerial("Next");
      }
    }
  }

  // Button 4 pressed.
  if (button4Debouncer.update())
  {
    //////  (  OLD WAY Works but not as stabel as hoped ) the un stable ness comes from debouncetime in press   //////
    // single or double  pressed
    //-----------------------------------------------------------------------------------------
    reading = digitalRead(buttonPin4);
    if (reading == HIGH && lastReading == LOW) // This is when the button is not pressed then the ontime is
    {
      onTime = millis();
    }
    else if (reading == LOW && lastReading == HIGH) // This is when the button is pressed
    {
      //    onTime = millis();
      if (((millis() - onTime) > bounceTime) && hold != 1)
      {
        press();
      }
      if (hold = 1) /// DOES THIS WORK TODO FIX IT
      {
        hold = 0;
      }
    }
  }

  lastReading = reading;
}
