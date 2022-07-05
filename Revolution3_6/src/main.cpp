#include <Arduino.h>
#include "QList.h"
#define FASTLED_INTERRUPT_RETRY_COUNT 2
// #define FASTLED_ALLOW_INTERRUPTS 0 // Needed to fix jitter/fluttering!
#include "FastLED.h"

#define LED_TYPE WS2812B
#define NUM_LEDS 150 //
#define COLOR_ORDER GRB
#define DATA_PIN 2 // LED Signal - Pin2 on the board.

CRGB leds[NUM_LEDS];
#define BRIGHTNESS 100

// Hall Sensor Note: Left-Grnd / Cntr-Power/ Right-Signal

uint16_t holdTime = 30;  // Milliseconds to hold position before advancing.
uint16_t spacing = 3;    // Sets pixel spacing. [Use 2 or greater] ( spacing = <3 WORKS good)
uint16_t delta = 5;      // Sets forward or backwards direction amount. (Can be negative.) (delta = 1 WORKS)
uint16_t width = 1;      // Can increase the number of pixels (width) of the chase. [1 or greater] (width = 1 WORKS)
uint8_t hue = 180;       // Starting color.
uint8_t hue2_shift = 20; // Hue shift for secondary color.  Use 0 for no shift. [0-255]

int16_t pos;                  // Pixel position.
int16_t prev_pos;             // previous Pixel position.
int16_t advance = -1 * width; // Stores the advance amount.
uint8_t color;                // Stores a hue color.
bool toggle = false;

long cycleCount = 0; // used for printing how many times thru the complete loop cycle

const int hallPin = 3;  // Hall Sensor Pin3 on the Board.
int hallState = 0;      // Hall Sensor
int prev_hallState = 0; // Hall Sensor
QList<bool> eventList;  // has all interrupt events
bool lock = false;
byte potPin = A0;
int potVal = analogRead(A0);

void interruptHall()
{
  Serial.println("hall sensor event --------------------");
  Serial.println(eventList.length());
  Serial.println(lock ? "true" : "false");

  // make sure there is only one event per magnet
  if (toggle == false)
  {
    toggle = true;
  }
  else
  {
    toggle = false;
    eventList.push_back(true);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup Start");
  FastLED.clear();
  delay(1600); // Startup delay

  pinMode(hallPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // setup Interrupt
  attachInterrupt(digitalPinToInterrupt(hallPin), interruptHall, CHANGE);

  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Blue;
  FastLED.show();
  // wait to connect the serial monitor
  delay(10000);

  Serial.print("NUM_LEDS/spacing: ");
  Serial.println(NUM_LEDS / spacing);
  Serial.println("Setup done. ");
}

// void loop()
// {

//   hallState = digitalRead(hallPin);
//   if (hallState != prev_hallState)
//   {

//     EVERY_N_MILLISECONDS(holdTime) // Advance pixels to next position.
//     {
//       ledCodeOnHalEvent();
//       prev_hallState = hallState; // Hall Sensor
//     }                             // Hall Sensor

//   } // end_every_n

// } // end_main_loop

void RainbowOnEvent()
{
  for (int i = 0; i < NUM_LEDS; ++i)
  {
    leds[i] = CHSV(hue + (i * 10), 255, 255);
  }
}
void RainbowOnLoop()
{
  hue++;
}

// set delta to 1 to work
void simpleBlueAndWhiteRunningWithSpacingAndWidth()
{
  for (int i = 0; i < NUM_LEDS; i = +(spacing + width))
  {
    if ((i + pos) % 2)
    {
      leds[i] = CRGB::Black;
    }
    else
    {
      leds[i] = CRGB::Blue;
    }
  }
}
// set delta to 1 to work
void simpleBlueAndWhiteRunningWithSpacing()
{
  for (int i = 0; i < NUM_LEDS; i = (i + spacing))
  {
    if ((i + pos) % 2)
    {
      leds[i] = CRGB::Black;
    }
    else
    {
      leds[i] = CRGB::Blue;
    }
  }
}
// set delta to 1 to work
void simpleBlueAndWhiteRunning()
{
  for (int i = 0; i < NUM_LEDS; ++i)
  {
    if ((i + pos) % 2)
    {
      leds[i] = CRGB::Black;
    }
    else
    {
      leds[i] = CRGB::Blue;
    }
  }
}
// set delta to 1 to work
void simpleGradientBlink()
{
  if (pos % 2)
    fill_gradient_RGB(leds, NUM_LEDS, CRGB::Magenta, CRGB::Yellow);
  else
    fill_gradient_RGB(leds, NUM_LEDS, CRGB::Red, CRGB::Blue);
}

#define SNAKE_LENGTH 10
#define SNAKE_COLOR CRGB::Red
#define SNAKE_BACKGROUND_COLOR CRGB::Black

// set delta to any number
void simpleSnakeRunning()
{
  // paint background
  for (int i = 0; i < NUM_LEDS; i++)
  {

    leds[i] = SNAKE_BACKGROUND_COLOR;
  }

  if (pos > SNAKE_LENGTH)
  {
    // snake until the end
    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
      leds[pos - i] = SNAKE_COLOR;
    }
  }
  else
  {
    //loop for the few LEds on the beginning
    for (int i = pos; i >= 0; i--)
    {
      leds[i] = SNAKE_COLOR;
    }
    // loop for the few LEds on the beginning
    for (int i = pos + NUM_LEDS - SNAKE_LENGTH; i < NUM_LEDS; i++)
    {
      leds[i] = SNAKE_COLOR;
    }
  }
}

void simpleWhiteDotRunning()
{
  leds[prev_pos] = CRGB::Black;
  leds[pos] = CRGB::White;
  FastLED.show();
}

void ledCodeOnHalEvent()
{
  simpleSnakeRunning();
}
void ledCodeOnLoop()
{
}

void loop()
{
  // update position

  if (lock == false)
  {

    // lock is making sure that the led routine gets not entered when it
    // is already running (avoid crash and reentrance)
    lock = true;

    ledCodeOnLoop();

    // potVal = analogRead(potPin);
    // Serial.println("potVal5:");
    // Serial.println(potVal);

    if (eventList.length() != 0)
    {
      // Advance pixels to next position.
      prev_pos = pos;
      if (pos < NUM_LEDS)
        pos = pos + delta;
      else
        pos = 0;

      ledCodeOnHalEvent();
      FastLED.show();

      Serial.println("led-done");
      Serial.print("pos:");
      Serial.println(pos);

      // que makes sure that we dont miss interrupt events
      eventList.pop_back();
      digitalToggleFast(LED_BUILTIN);
    }

    lock = false;
  }
}

