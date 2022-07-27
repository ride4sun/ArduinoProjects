#include <Arduino.h>
#include <candyCane.hpp>
#include <rainbow2.hpp>
#include <snake.h>
#include <whiteDotRunning.h>
#include <blueAndWhite.h>
#include <juggle.h>

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

CRGB ledsOne[NUM_LEDS_ONE]; // the led array representing the leds addressable by FASTLED
#ifdef LED_STRING_TWO_PRESENT
CRGB ledsTwo[NUM_LEDS_TWO]; // the led array representing the leds addressable by FASTLED
#endif

ledFunction ledFunctionsOne[12]; // the number of different led animations for the potentiometer to select
#ifdef LED_STRING_TWO_PRESENT
ledFunction ledFunctionsTwo[12]; // the number of different led animations for the potentiometer to select
#endif

uint16_t deltaOne = 3; // Sets forward or backwards direction amount. (Can be negative.) (delta = 1 WORKS)
#ifdef LED_STRING_TWO_PRESENT
uint16_t deltaTwo = 2; // Sets forward or backwards direction amount. (Can be negative.) (delta = 1 WORKS)
#endif

bool startUpDone = false;

struct ledData ledDataOne;
#ifdef LED_STRING_TWO_PRESENT
struct ledData ledDataTwo;
#endif

bool toggle = false; // used to limit the number of hall events per magnet to one
bool reverse = false; // the direction of the hall event

QList<bool> eventList; // has all interrupt events
bool lock = false;
byte potPin = A0;
int potVal = analogRead(A0);
bool hallSensorActive = true;
bool simulateHallSensorEvents = true;

void interruptHall()
{
  if (startUpDone == false)
  {
    return;
  }

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
//RainbowLedAnimation *rainbowAnimation = new RainbowLedAnimation();
CandyCaneAnimation *animation = new CandyCaneAnimation();

void setupLedFunctions()
{
  //ledFunctionsOne[0] = {NULL, NULL, snakeHallEvent, 1};
  
  // ledFunctionsOne[0] = {NULL, NULL, juggleHallEvent, 1};
  // ledFunctionsOne[1] = {NULL, NULL, whiteDotEvent, 1};
  // ledFunctionsOne[2] = {NULL, NULL, candyCaneHallEvent, 1};
  // ledFunctionsOne[3] = {NULL, rainbowLoop, rainbowHallEvent, 1};
  // ledFunctionsOne[NUMBER_OF_ANIMATIONS - 1] = {NULL, NULL, blueAndWhiteEvent, 1};

#ifdef LED_STRING_TWO_PRESENT

  //ledFunctionsTwo[0] = {NULL, NULL, snakeHallEvent, 1};
  
  // ledFunctionsTwo[0] = {NULL, NULL, juggleHallEvent, 1};
  // ledFunctionsTwo[1] = {NULL, NULL, whiteDotEvent, 1};
  // ledFunctionsTwo[2] = {NULL, NULL, candyCaneHallEvent, 1};
  // ledFunctionsTwo[3] = {NULL, rainbowLoop, rainbowHallEvent, 1};
  // ledFunctionsTwo[NUMBER_OF_ANIMATIONS - 1] = {NULL, NULL, blueAndWhiteEvent, 1};
#endif  

}
void ledCodeOnHalEvent()
{

  animation->OnHallEvent(ledDataOne);

  // ledFunction selectedFunction = ledFunctionsOne[potentiometerPosition];
  // if (selectedFunction.LedFunctionHallEvent != NULL)
  //   selectedFunction.LedFunctionHallEvent(ledDataOne);

#ifdef LED_STRING_TWO_PRESENT
      selectedFunction = ledFunctionsTwo[potentiometerPosition];
  if (selectedFunction.LedFunctionHallEvent != NULL)
    selectedFunction.LedFunctionHallEvent(ledDataTwo);
#endif
}

void ledCodeOnLoop()
{
  animation->OnFastLoop();
  // ledFunction selectedFunction = ledFunctionsOne[potentiometerPosition];
  // if (selectedFunction.LedFunctionFastLoop != NULL)
  //   selectedFunction.LedFunctionFastLoop();

#ifdef LED_STRING_TWO_PRESENT
  selectedFunction = ledFunctionsTwo[potentiometerPosition];
  if (selectedFunction.LedFunctionFastLoop != NULL)
    selectedFunction.LedFunctionFastLoop();
#endif    
}

void ledCodeOnSetup()
{
  animation->OnSetup();

  // ledFunction selectedFunction = ledFunctionsOne[potentiometerPosition];
  // if (selectedFunction.LedFunctionSetup != NULL)
  //   selectedFunction.LedFunctionSetup();

#ifdef LED_STRING_TWO_PRESENT
  selectedFunction = ledFunctionsTwo[potentiometerPosition];
  if (selectedFunction.LedFunctionSetup != NULL)
    selectedFunction.LedFunctionSetup();
#endif
}

void showStartupAnimation()
{
  ledsOne[0] = CRGB::Red;
#ifdef LED_STRING_TWO_PRESENT
  ledsTwo[0] = CRGB::Red;
#endif
  FastLED.show();

  delay(2500);
  ledsOne[1] = CRGB::Orange;
#ifdef LED_STRING_TWO_PRESENT
  ledsTwo[1] = CRGB::Orange;
#endif
  FastLED.show();

  delay(2500);
  ledsOne[2] = CRGB::Yellow;
#ifdef LED_STRING_TWO_PRESENT
  ledsTwo[2] = CRGB::Yellow;
#endif
  FastLED.show();

  delay(2500);
  ledsOne[3] = CRGB::Green;
#ifdef LED_STRING_TWO_PRESENT
  ledsTwo[3] = CRGB::Green;
#endif

  FastLED.show();
  delay(2500);
  FastLED.clear();
}

void setup()
{
  ledDataOne = {ledsOne, 0, 0, NUM_LEDS_ONE};
#ifdef LED_STRING_TWO_PRESENT
  ledDataTwo = {ledsTwo, 0, 0, NUM_LEDS_TWO};
#endif


  setupLedFunctions();

  Serial.begin(115200);
  Serial.println("Setup start --------------------------------------");
  FastLED.clear();
  delay(1600); // Startup delay

  pinMode(HALL_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // setup Interrupt
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), interruptHall, CHANGE);

  Serial.println("1 -------------");

  FastLED.addLeds<LED_TYPE, DATA_PIN_ONE, GRB>(ledsOne, NUM_LEDS_ONE);
#ifdef LED_STRING_TWO_PRESENT
  FastLED.addLeds<LED_TYPE, DATA_PIN_TWO, GRB>(ledsTwo, NUM_LEDS_TWO);
#endif

  // 4 Wire LED setup
  // FastLED.addLeds<LED_TYPE, DATA_PIN_ONE, CLK_PIN, COLOR_ORDER>(ledsOne, NUM_LEDS_ONE).setCorrection(TypicalLEDStrip); // with clock_pin

  Serial.println("2 -------------");

  FastLED.setBrightness(BRIGHTNESS);

  Serial.println("3 -------------");

  showStartupAnimation();

  Serial.println("4 -------------");

  // ledFunction selectedFunction = ledFunctionsOne[potentiometerPosition];
  // if (selectedFunction.delta != NULL)
  //   deltaOne = selectedFunction.delta;

  Serial.println("5 -------------");

#ifdef LED_STRING_TWO_PRESENT
  // selectedFunction = ledFunctionsTwo[potentiometerPosition];
  // if (selectedFunction.delta != NULL)
  //   deltaTwo = selectedFunction.delta;
#endif

  ledCodeOnSetup();

  Serial.println("6 -------------");

  Serial.print("NUM_LEDS_ONE: ");
  Serial.println(NUM_LEDS_ONE);
  Serial.print("no Of Leds: ");
  Serial.println(ledDataOne.noOfLeds);

  Serial.println("Setup done --------------------------------------");

  startUpDone = true;
}

void UpdatePosition(struct ledData &data, uint16_t delta)
{

  Serial.print("pos one before:");
  Serial.println(data.pos);

  data.lastPos = data.pos;

  if (reverse)
  {
    // backwards
    if (delta > data.pos)
    {
      data.pos = data.noOfLeds - (delta - data.pos);
    }
    else
    {
      data.pos = data.pos - delta;
    }
  }
  else
  {
    // forwards
    if ((data.pos + delta) < data.noOfLeds)
    {
      data.pos = data.pos + delta;
      // Serial.print("pos + delta:");
      // Serial.println(pos);
    }
    else
    {
      data.pos = data.pos + delta - data.noOfLeds;
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

    // ledCodeOnSetup();
  }

  // no need to read the Potentiometer often
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

  EVERY_N_MILLISECONDS(SIMULATED_INTERRUPT_TIME) // Advance pixels to next position.
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
      UpdatePosition(ledDataOne, deltaOne);
#ifdef LED_STRING_TWO_PRESENT
      UpdatePosition(ledDataTwo, deltaTwo);
#endif
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
