
//***************************************************************

#include <QList.h>
#include "FastLED.h"

#define LED_TYPE WS2812B
#define NUM_LEDS 300 //
#define COLOR_ORDER GRB
#define DATA_PIN 2 //LED Signal - Pin2 on the board.

CRGB leds[NUM_LEDS];
#define BRIGHTNESS 100

//Hall Sensor Note: Left-Grnd / Cntr-Power/ Right-Signal

uint16_t holdTime = 1;   // Milliseconds to hold position before advancing.
uint16_t spacing = 4;    // Sets pixel spacing. [Use 2 or greater] ( spacing = <3 WORKS good)
uint16_t delta = 1;      // Sets forward or backwards direction amount. (Can be negative.) (delta = 1 WORKS)
uint16_t width = 1;      // Can increase the number of pixels (width) of the chase. [1 or greater] (width = 1 WORKS)
uint8_t hue = 180;       // Starting color.
uint8_t hue2_shift = 20; // Hue shift for secondary color.  Use 0 for no shift. [0-255]

int16_t pos;                  // Pixel position.
int16_t advance = -1 * width; // Stores the advance amount.
uint8_t color;                // Stores a hue color.

long cycleCount = 0; // used for printing how many times thru the complete loop cycle

const int hallPin = 3;  //Hall Sensor Pin3 on the Board.
int hallState = 0;      //Hall Sensor
int prev_hallState = 0; //Hall Sensor
QList<bool> eventList; //has all interrupt events
bool lock = false;

//---------------------------------------------------------------
void setup()
{
  pinMode(hallPin, INPUT);

  FastLED.clear(); // clear all pixel data

  Serial.begin(9600); // Allows serial monitor output (check baud rate)
  delay(1600);        // Startup delay

  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  //setup Interupt
  attachInterrupt(digitalPinToInterrupt(hallPin), interruptHall, CHANGE);

  Serial.print("NUM_LEDS/spacing: ");
  Serial.println(NUM_LEDS / spacing);
  Serial.println("Setup done. ");
}

void interruptHall()
{
  eventList.push_front(true);
}

//Main Timer loop
void loop()
{
  EVERY_N_MILLISECONDS(holdTime) 
  {
    if(eventList.length() != 0)
    {

      // lock is making sure that the led routine gets not entered when it 
      // is already running (avoid crash and reentrance)
      if(lock == false)
      {
        lock = true;
        // Advance pixels to next position.
        ledCode();
        //que makes sure that we dont miss interrupt events
        eventList.pop_back();
        lock = false;
      }
    }
  } 
} 




void ledCode()
{
  // Advance pixel postion down strip, and rollover if needed.
  // advance = (advance + delta + NUM_LEDS) % NUM_LEDS; // orginal code to advance forward or backward with -1
  advance = (advance + 1 + NUM_LEDS) % NUM_LEDS; // John modification - only advance forward '1'.
  {
    for (uint16_t i = 0; i < (NUM_LEDS / spacing); i++)
    {
      for (uint16_t w = 0; w < width; w++)
      {

        pos = (spacing * (i - 1) + spacing + advance + w - 1) % NUM_LEDS;
        leds[pos] = CRGB::Black; // Secondary Dot color
                                 // leds[pos] = CRGB::color2;
                                 //          Serial.print(" "); Serial.print(pos); Serial.print("  ");
      }
      //    Serial.println(" ");
    }
    //    Serial.println(" ");
    //   Serial.print (" Cycle Count = ");
    //   Serial.println (cycleCount);
    cycleCount = cycleCount + 1;
  }
  // Update pixels down the strip.
  for (uint16_t i = 0; i < (NUM_LEDS / spacing); i++)
  {
    for (uint16_t w = 0; w < width; w++)
    {
      pos = (spacing * (i - 1) + spacing + advance + w) % NUM_LEDS;
      if (w % 2 == 0)
      {              // Is w even or odd? //\\error: want to clean out this code, but sketch will crash after a while.
        color = hue; // \\error: want to clean out this code, but sketch will crash after a while.
      }
      else
      {                           // \\error: want to clean out this code, but sketch will crash after a while.
        color = hue + hue2_shift; //  \\error: want to clean out this code, but sketch will crash after a while.
      }
      leds[pos] = CRGB::Blue; //Primary Dot color
    }
  }

  FastLED.show();
}
