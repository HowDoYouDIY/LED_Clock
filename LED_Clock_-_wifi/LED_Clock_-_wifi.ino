#include "Arduino.h"
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#include "WifiConnect.h"
#include "InternetTime.h"

// LED strip constants
#define LED_PIN   2 // D4 pin on the Wemos D1 Mini
#define NUM_LEDS  60
CRGB leds[NUM_LEDS];

//************************************************************************
// Your config below!
//************************************************************************
const char* WIFI_NAME     = "Your WIFI Name";     // SSID of local network
const char* WIFI_PASSWORD = "Your WIFI Password"; // Password on network
long utcOffset = 0;                               // Hour adjustment from the found Internet Time
//************************************************************************
bool timeFound = false;
bool needToUpdateTime = true;
int hour = 0;
int minute = 0;
int second = 0;
int ledSetupColor = 0;

#define MILLIS_DELAYED 1000
unsigned long delayMillis = 0;
//************************************************************************


void setup() 
{
  Serial.begin(115200);

  if (utcOffset < 0) {
    utcOffset = utcOffset + 12;
  }
  connectToWifi(WIFI_NAME, WIFI_PASSWORD);
  startUdp();
  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  ledSetup();
  while (!timeFound) {
    getTime();
  }
  delayMillis = millis();
}

void loop()
{
  setLeds();
  FastLED.show();
    
  while ((delayMillis - millis()) < MILLIS_DELAYED) {
    /*********************************************************************
     * This is the delay part of a custom delay function. It waits a more 
     * accurate delay time in the entire cycle of the program rather than 
     * just delay from this moment. The amount of time delayed is set by 
     * the constant MILLIS_DELAYED.
     * 
     * It's basically just a "Do Nothing" loop based on a time that was 
     * set externally with the delayMillis variable.
     *********************************************************************/
  }
  Serial.print(millis());
  Serial.print(", ");
  Serial.println(delayMillis);
  delay(100);
}


//************************************************************************
//*** Setup Functions
//************************************************************************

/** Comment out if not using the wifi board **/
void ledSetup()
{
  /***********************************************************************
   * This for loop sets all of the LEDs to a single color in sequence, 
   * from beginning to end. It cycles as it tries to find the current
   * time from the internet. The color changes with each query of the
   * time servers. The faster it gets the time, the fewer color cycles 
   * it goes through.
   ***********************************************************************/
  for (int i = 0; i < NUM_LEDS; i++) {
    switch (ledSetupColor) {
      case 1:  leds[i] = CRGB (0, 5, 5);  // max value is 255
               break;
      case 2:  leds[i] = CRGB (0, 10, 0);  // max value is 255
               break;
      case 3:  leds[i] = CRGB (5, 5, 0);  // max value is 255
               break;
      case 4:  leds[i] = CRGB (10, 0, 0);  // max value is 255
               break;
      case 5:  leds[i] = CRGB (5, 0, 5);  // max value is 255
               break;
      default: leds[i] = CRGB (0, 0, 10);  // max value is 255
    }
    FastLED.show();
    delay(25);
  }
  ledSetupColor++;
  if (ledSetupColor > 5) {
    ledSetupColor = 0;
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
    second -= 60;     // reset the second hand,
    minute++;         // and advance the minute hand.
  }

  if (minute > 59) { // at 60 minutes,
    minute -= 60;    // reset the minute hand,
    hour++;          // and advance the hour hand.
    needToUpdateTime = true;  // Get ready to update the time 
  }

  if (hour > 11) {    // at 12 hours,
    hour -= 12;       // reset the hour hand,
//    timeFound = false;
    if (hour <= 0 && needToUpdateTime) { 
      getTime();      // and refresh the time.
      if (timeFound) {             // Once the updated time is found
        needToUpdateTime = false;  // then don't try to update again this hour.
      }
    }
  }

  leds[(hour * 5) + (minute / 12)] = CRGB (150, 0, 0);  // <--- display hour hand
  leds[minute] = CRGB (100, 40, 0);  // <---------------------- display minute hand
  leds[second] = CRGB (100, 100, 0);  // <--------------------- display second hand

  second++;
  delayMillis += MILLIS_DELAYED;
}

void setAllLeds(int r, int g, int b)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB (r, g, b); // max value is 255          
  }
}

void getTime(void) {
  timeFound = getTime(&hour, &minute, &second);
  if (!timeFound) {
    ledSetup(); // Show cycling colors while trying to find the time.
  } else {
    second += 6; // Adjust the number of seconds since retrieved.
    hour += utcOffset; // Adjust the hour for my local time zone.
  }

/*************************************************************************
 * This next section is only for Serial Monitor Output purposes and does
 * not affect the function of the clock in any way. It can be commented 
 * out entirely by removing the / at the end if this comment header section.
 *************************************************************************/
  Serial.println();
  Serial.print("The local time is ");
  Serial.print(hour); // print local hour
  Serial.print(':');
  if (minute < 10) {
    // In the first 10 minutes of each hour, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.print(minute); // print the minute (3600 equals secs per minute)
  Serial.print(':');
  if (second < 10) {
    // In the first 10 seconds of each minute, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.println(second); // print the second
/*************************************************************************/

}
