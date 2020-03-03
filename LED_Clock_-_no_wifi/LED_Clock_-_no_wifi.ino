#include "Arduino.h"
#include <FastLED.h>

// Capacitive touch pins
#define HOUR_PIN 2 // D2 on the Arduino Nano
#define MINUTE_PIN 3 // D3 on the Arduino Nano

// LED strip constants
#define LED_PIN   4 // D4 on the Arduino Nano
#define NUM_LEDS  60
CRGB leds[NUM_LEDS];

//************************************************************************
long utcOffset = -7; // This is the time zone offset

int hour = 0;
int minute = 0;
int second = 0;

bool pauseClock = false;
unsigned long pausedMillis = 0;
unsigned long previousMillis = 10; // This variable is unnecessary, but removing it breaks the code for some reason
//************************************************************************


void setup() 
{
  if (utcOffset < 0) {
    utcOffset = utcOffset + 12;
  }

  pinMode(HOUR_PIN, INPUT);
  pinMode(MINUTE_PIN, INPUT);
  Serial.begin(9600);
  
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  ledSetup();
}

void loop()
{
  if (digitalRead(HOUR_PIN) == HIGH) {
    incrementTime(HOUR_PIN);
    Serial.println("HOUR Sensor is touched");
  }
  if (digitalRead(MINUTE_PIN) == HIGH) {
    incrementTime(MINUTE_PIN);
    Serial.println("MINUTE Sensor is touched");
  }
  
  setLeds();
  FastLED.show();
  if (pauseClock) {
    if ((millis() - pausedMillis) > 1000) {
      pauseClock = false;
    }
    delay(1);
    Serial.println(millis());
  } else {
    delay(998);
    Serial.println(millis());
  }
}


//************************************************************************
//*** Setup Functions
//************************************************************************

void ledSetup()
{
  /* This for loop sets all of the LEDs to blue in sequence, beginning to end. 
   */
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB (0, 0, 9);  // max value is 255
    FastLED.show();
    delay(40);
  }
}


//************************************************************************
//*** Running Functions
//************************************************************************

void setLeds()
{
  setAllLeds(0, 0, 3);  // display for each of the minute leds

  for (int i = 0; i < 12; i++) {  // display for each of the hour leds 
    leds[i * 5] = CRGB (0, 5, 0);
  }

  if (second > 59) {  // at 60 second,
    second = 0;       // reset the second hand,
    minute++;         // and advance the minute hand.
  }

  if (minute > 59) {  // at 60 minutes,
    minute = 0;       // reset the minute hand,
    hour++;           // and advance the hour hand.
  }

  if (hour > 11) {  // at 12 hours,
    hour = 0;       // reset the hour hand,
  }

  leds[(hour * 5) + (minute / 12)] = CRGB (150, 0, 0);  // display hour hand
  leds[minute] = CRGB (100, 40, 0);  // display minute hand
  leds[second] = CRGB (100, 100, 0);  // display second hand

  if (!pauseClock) {
    if ((millis() - previousMillis) > 1000) {
      second++;
      previousMillis = 0;
    }
  }
}

void setAllLeds(int r, int g, int b)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB (r, g, b); // max value is 255          
  }
}

void incrementTime (const int HAND)
{
  pauseClock = true;
  pausedMillis = millis();
  if (HAND == HOUR_PIN) {
    hour++;
  } else if (HAND == MINUTE_PIN) {
    minute++;
  }
  delay(500);
}
