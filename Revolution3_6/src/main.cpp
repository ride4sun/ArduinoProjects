#include <Arduino.h>
#include <candyCane.h>
#include <rainbow.h>
#include <snake.h>
#include <whiteDotRunning.h>
#include <blueAndWhite.h>
#include "QList.h"
#include "defines.h"
#define FASTLED_INTERRUPT_RETRY_COUNT 2
// #define FASTLED_ALLOW_INTERRUPTS 0 // Needed to fix jitter/fluttering!
#include "FastLED.h"

typedef struct ledFunction
{
  void (*LedFunctionSetup)();
  void (*LedFunctionFastLoop)();
  void (*LedFunctionHallEvent)(CRGB *, uint16_t, uint16_t);
  uint16_t delta;
} ledMethods;

CRGB leds[NUM_LEDS];          // the led array representing the leds addressable by FASTLED
ledFunction ledFunctions[12]; // the number of different led animations for the potentiometer to select

uint16_t delta = 5; // Sets forward or backwards direction amount. (Can be negative.) (delta = 1 WORKS)
uint16_t pos;       // Pixel position.
uint16_t prev_pos;  // previous Pixel position.

bool toggle = false; // used to limit the number of hall events per magnet to one
bool reverse = true; // the direction of the hall event

const int hallPin = 3; // Hall Sensor Pin3 on the Board.
QList<bool> eventList; // has all interrupt events
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

int potentiometerPosition = 4;

void setupLedFunctions()
{
  ledFunctions[0] = {NULL, NULL, candyCaneHallEvent, 5};
  ledFunctions[1] = {NULL, NULL, snakeHallEvent, 5};
  ledFunctions[2] = {NULL, rainbowLoop, rainbowHallEvent, 5};
  ledFunctions[3] = {NULL, NULL, whiteDotEvent, 2};
  ledFunctions[NUMBER_OF_ANIMATIONS - 1] = {NULL, NULL, blueAndWhiteEvent, 1};
}
void ledCodeOnHalEvent()
{
  ledFunction selectedFunction = ledFunctions[potentiometerPosition];
  if (selectedFunction.LedFunctionHallEvent != NULL)
    selectedFunction.LedFunctionHallEvent(leds, pos, prev_pos);
}

void ledCodeOnLoop()
{
  ledFunction selectedFunction = ledFunctions[potentiometerPosition];
  if (selectedFunction.LedFunctionFastLoop != NULL)
    selectedFunction.LedFunctionFastLoop();
}

void ledCodeOnSetup()
{
  ledFunction selectedFunction = ledFunctions[potentiometerPosition];
  if (selectedFunction.LedFunctionSetup != NULL)
    selectedFunction.LedFunctionSetup();
}

void showStartupAnimation()
{
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(2500);
  leds[1] = CRGB::Orange;
  FastLED.show();
  delay(2500);
  leds[2] = CRGB::Yellow;
  FastLED.show();
  delay(2500);
  leds[3] = CRGB::Green;
  FastLED.show();
  delay(2500);
  FastLED.clear();
}

void setup()
{
  setupLedFunctions();

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

  showStartupAnimation();

  ledFunction selectedFunction = ledFunctions[potentiometerPosition];
  if (selectedFunction.delta != NULL)
    delta = selectedFunction.delta;

  ledCodeOnSetup();

  Serial.print("NUM_LEDS: ");
  Serial.println(NUM_LEDS);
  Serial.println("Setup done. ");
}

void simpleGradientBlink()
{
  if (pos % 2)
    fill_gradient_RGB(leds, NUM_LEDS, CRGB::Magenta, CRGB::Yellow);
  else
    fill_gradient_RGB(leds, NUM_LEDS, CRGB::Red, CRGB::Blue);
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
  // slow updates here ...
  EVERY_N_MILLISECONDS(60000) // Advance pixels to next position.
  {
    if (potentiometerPosition < (NUMBER_OF_ANIMATIONS - 1))
      potentiometerPosition++;
    else
      potentiometerPosition = 0;

    ledCodeOnSetup();
  }

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
