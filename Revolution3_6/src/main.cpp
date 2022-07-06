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
uint16_t delta = 1;      // Sets forward or backwards direction amount. (Can be negative.) (delta = 1 WORKS)
uint16_t width = 1;      // Can increase the number of pixels (width) of the chase. [1 or greater] (width = 1 WORKS)
uint8_t hue = 0;         // Starting color.
uint8_t hue2_shift = 20; // Hue shift for secondary color.  Use 0 for no shift. [0-255]

uint16_t pos;                 // Pixel position.
uint16_t prev_pos;            // previous Pixel position.
int16_t advance = -1 * width; // Stores the advance amount.
uint8_t color;                // Stores a hue color.
bool toggle = false;
bool reverse = true;

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
  Serial.println("------------------------------>");
  Serial.println(eventList.length());
  Serial.print("lock:");
  Serial.println(lock ? "true" : "false");

  // make sure there is only one event per magnet
  if (toggle == false)
  {
    toggle = true;
    eventList.push_back(true);
  }
  else
  {
    toggle = false;
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


int shiftBy = 1;                  // shiftBy can be positive or negative (to change direction)
static uint16_t numColors = 2;    // Can be either 2 or 3
static uint16_t stripeLength = 4; // number of pixels per color
static int offset;

CRGB color1 = CRGB::Blue; // color used between color 2 (and 3 if used)
CRGB color2 = CRGB::Black;

// set delta to any number
void candyCane()
{

  // Below is the jonniji modification to the loop.
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    if ((i + offset) % ((numColors)*stripeLength) < stripeLength)
    {
      leds[i] = color2;
    }
    else
    {
      leds[i] = color1;
    }
  }
  // This is the section of code that makes the lights move

  offset = offset + shiftBy;
  if (shiftBy > 0)
  { // for positive shiftBy
    if (offset >= NUM_LEDS)
      offset = 0;
  }
  else
  { // for negitive shiftBy
    if (offset < 0)
      offset = NUM_LEDS;
  }
}

#define SNAKE_LENGTH 100
#define SNAKE_BACKGROUND_COLOR CRGB::Black
// set delta to any number
void simpleSnakeRunning()
{

  uint8_t oldHue = hue;

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
      leds[pos - i] = CHSV(hue++, 255, 255);
    }
  }
  else
  {
    // loop for the few LEds on the beginning
    for (int i = pos; i >= 0; i--)
    {
      leds[i] = CHSV(hue++, 255, 255);
    }
    // loop for the few LEds on the beginning
    for (int i = pos + NUM_LEDS - SNAKE_LENGTH; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV(hue++, 255, 255);
    }
  }
  hue = oldHue + 10;
}

void simpleWhiteDotRunning()
{
  leds[prev_pos] = CRGB::Black;
  leds[pos] = CRGB::White;
  FastLED.show();
}

void ledCodeOnHalEvent()
{
  candyCane();
}
void ledCodeOnLoop()
{
}

void UpdatePosition()
{

  Serial.print("pos before:");
  Serial.println(pos);

  prev_pos = pos;
  if (reverse)
  {
    // backwards
    if (delta > pos)
    {
      pos = NUM_LEDS - (delta - pos);
    }
    else
    {
      pos = pos - delta;
    }
  }
  else
  {
    // forwards
    if ((pos + delta) < NUM_LEDS)
    {
      pos = pos + delta;
      // Serial.print("pos + delta:");
      // Serial.println(pos);
    }
    else
    {
      pos = pos + delta - NUM_LEDS;
      // Serial.print("pos + delta - NUM_LEDS:");
      // Serial.println(pos);
    }
  }
}

void loop()
{
  // update position

  // slow updates here ...
  EVERY_N_MILLISECONDS(5000) // Advance pixels to next position.
  {
    // potVal = analogRead(potPin);
    // Serial.print("potVal:");
    // Serial.println(potVal);
  }

  if (lock == false)
  {

    // lock is making sure that the led routine gets not entered when it
    // is already running (avoid crash and reentrance)
    lock = true;

    ledCodeOnLoop();

    if (eventList.length() != 0)
    {
      UpdatePosition();
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
