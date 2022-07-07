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
  void (*LedFunctionHallEvent)(struct ledData);
  uint16_t delta;
} ledMethods;

CRGB ledsOne[NUM_LEDS_ONE];      // the led array representing the leds addressable by FASTLED
CRGB ledsTwo[NUM_LEDS_TWO];      // the led array representing the leds addressable by FASTLED

ledFunction ledFunctionsOne[12]; // the number of different led animations for the potentiometer to select
ledFunction ledFunctionsTwo[12]; // the number of different led animations for the potentiometer to select

uint16_t deltaOne = 1; // Sets forward or backwards direction amount. (Can be negative.) (delta = 1 WORKS)
uint16_t deltaTwo = 1; // Sets forward or backwards direction amount. (Can be negative.) (delta = 1 WORKS)

struct ledData ledDataOne = {ledsOne, 0, 0, NUM_LEDS_ONE};
struct ledData ledDataTwo = {ledsTwo, 0, 0, NUM_LEDS_TWO};

bool toggle = false; // used to limit the number of hall events per magnet to one
bool reverse = true; // the direction of the hall event

QList<bool> eventList; // has all interrupt events
bool lock = false;
byte potPin = A0;
int potVal = analogRead(A0);
bool hallSensorActive = true;
bool simulateHallSensorEvents = true;

void interruptHall()
{
  hallSensorActive = true;
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

  Serial.println("------------------------------>");
  Serial.println(eventList.length());
  Serial.print("lock:");
  Serial.println(lock ? "true" : "false");
}
void interruptHallSimulated()
{
  eventList.push_back(true);
}

int potentiometerPosition = 0;

void setupLedFunctions()
{
  ledFunctionsOne[0] = {NULL, NULL, candyCaneHallEvent, 1};
  ledFunctionsOne[1] = {NULL, NULL, snakeHallEvent, 1};
  ledFunctionsOne[2] = {NULL, rainbowLoop, rainbowHallEvent, 1};
  ledFunctionsOne[3] = {NULL, NULL, whiteDotEvent, 1};
  ledFunctionsOne[NUMBER_OF_ANIMATIONS - 1] = {NULL, NULL, blueAndWhiteEvent, 1};

  ledFunctionsTwo[0] = {NULL, NULL, candyCaneHallEvent, 1};
  ledFunctionsTwo[1] = {NULL, NULL, snakeHallEvent, 1};
  ledFunctionsTwo[2] = {NULL, rainbowLoop, rainbowHallEvent, 1};
  ledFunctionsTwo[3] = {NULL, NULL, whiteDotEvent, 1};
  ledFunctionsTwo[NUMBER_OF_ANIMATIONS - 1] = {NULL, NULL, blueAndWhiteEvent, 1};
}
void ledCodeOnHalEvent()
{
  ledFunction selectedFunction = ledFunctionsOne[potentiometerPosition];
  if (selectedFunction.LedFunctionHallEvent != NULL)
    selectedFunction.LedFunctionHallEvent(ledDataOne);

  selectedFunction = ledFunctionsTwo[potentiometerPosition];
  if (selectedFunction.LedFunctionHallEvent != NULL)
    selectedFunction.LedFunctionHallEvent(ledDataTwo);
}

void ledCodeOnLoop()
{
  ledFunction selectedFunction = ledFunctionsOne[potentiometerPosition];
  if (selectedFunction.LedFunctionFastLoop != NULL)
    selectedFunction.LedFunctionFastLoop();

  selectedFunction = ledFunctionsTwo[potentiometerPosition];
  if (selectedFunction.LedFunctionFastLoop != NULL)
    selectedFunction.LedFunctionFastLoop();
}

void ledCodeOnSetup()
{
  ledFunction selectedFunction = ledFunctionsOne[potentiometerPosition];
  if (selectedFunction.LedFunctionSetup != NULL)
    selectedFunction.LedFunctionSetup();

  selectedFunction = ledFunctionsTwo[potentiometerPosition];
  if (selectedFunction.LedFunctionSetup != NULL)
    selectedFunction.LedFunctionSetup();
}

void showStartupAnimation()
{
  ledsOne[0] = CRGB::Red;
  ledsTwo[0] = CRGB::Red;
  FastLED.show();
  delay(2500);
  ledsOne[1] = CRGB::Orange;
  ledsTwo[1] = CRGB::Orange;
  FastLED.show();
  delay(2500);
  ledsOne[2] = CRGB::Yellow;
  ledsTwo[2] = CRGB::Yellow;
  FastLED.show();
  delay(2500);
  ledsOne[3] = CRGB::Green;
  ledsTwo[3] = CRGB::Green;
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

  pinMode(HALL_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // setup Interrupt
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), interruptHall, CHANGE);

  FastLED.addLeds<LED_TYPE, DATA_PIN_ONE, GRB>(ledsOne, NUM_LEDS_ONE);
  FastLED.addLeds<LED_TYPE, DATA_PIN_TWO, GRB>(ledsTwo, NUM_LEDS_TWO);
  // FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // no clock_pin
  // FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // with clock_pin

  FastLED.setBrightness(BRIGHTNESS);

  showStartupAnimation();

  ledFunction selectedFunction = ledFunctionsOne[potentiometerPosition];
  if (selectedFunction.delta != NULL)
    deltaOne = selectedFunction.delta;

  selectedFunction = ledFunctionsTwo[potentiometerPosition];
  if (selectedFunction.delta != NULL)
    deltaTwo = selectedFunction.delta;

  ledCodeOnSetup();

  Serial.print("NUM_LEDS_ONE: ");
  Serial.println(NUM_LEDS_ONE);
  Serial.println("Setup done. ");
}

void UpdatePosition(struct ledData &data, uint8_t delta, uint16_t noOfLeds)
{

  Serial.print("pos one before:");
  Serial.println(data.pos);

  data.lastPos = data.pos;

  if (reverse)
  {
    // backwards
    if (delta > data.pos)
    {
      data.pos = noOfLeds - (delta - data.pos);
    }
    else
    {
      data.pos = data.pos - delta;
    }
  }
  else
  {
    // forwards
    if ((data.pos + delta) < noOfLeds)
    {
      data.pos = data.pos + delta;
      // Serial.print("pos + delta:");
      // Serial.println(pos);
    }
    else
    {
      data.pos = data.pos + delta - noOfLeds;
      // Serial.print("pos + delta - NUM_LEDS_ONE:");
      // Serial.println(pos);
    }
  }
  Serial.print("pos one after:");
  Serial.println(data.pos);
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

  //no need to read the Potentiometer often
  EVERY_N_MILLISECONDS(1000)
  {
    potVal = analogRead(potPin);
    Serial.print("potVal:");
    Serial.println(potVal);
  }

  EVERY_N_MILLISECONDS(5000)
  {
    if (hallSensorActive == true)
    {
      hallSensorActive = false;
      simulateHallSensorEvents = false;
    }
    else
    {
      simulateHallSensorEvents = true;
    }
  }

  EVERY_N_MILLISECONDS(50) // Advance pixels to next position.
  {
    if (simulateHallSensorEvents)
      interruptHallSimulated();
  }

  if (lock == false)
  {

    // lock is making sure that the led routine gets not entered when it
    // is already running (avoid crash and reentrance)
    lock = true;

    ledCodeOnLoop();

    if (eventList.length() != 0)
    {
      UpdatePosition(ledDataOne, deltaOne, NUM_LEDS_ONE);
      UpdatePosition(ledDataTwo, deltaTwo, NUM_LEDS_TWO);
      ledCodeOnHalEvent();
      FastLED.show();

      Serial.println("led done");

      // que makes sure that we dont miss interrupt events
      eventList.pop_back();
      // digitalToggleFast(LED_BUILTIN);
    }

    lock = false;
  }
}
