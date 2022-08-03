#include <Arduino.h>
#include <rotate.hpp>
#include <heartBeat.hpp>
#include <sinelon.hpp>
#include <bpm.hpp>
#include <candyCane.hpp>
#include <rainbow2.hpp>
#include <rainbow.hpp>
#include <snake.hpp>
#include <whiteDotRunning.hpp>
#include <blueAndWhite.hpp>
#include <juggle.hpp>

#include "QList.h"
#include "defines.h"
#define FASTLED_INTERRUPT_RETRY_COUNT 2
// #define FASTLED_ALLOW_INTERRUPTS 0 // Needed to fix jitter/fluttering!
#include "FastLED.h"

CRGB ledsOne[NUM_LEDS_ONE]; // the led array representing the leds addressable by FASTLED
#ifdef LED_STRING_TWO_PRESENT
CRGB ledsTwo[NUM_LEDS_TWO]; // the led array representing the leds addressable by FASTLED
#endif

uint8_t numberOfAnimations = 0;
uint8_t numberOfPotPositions = 0;

IAnimation *activeAnimationOne;
IAnimation *activeAnimationTwo;
IAnimation *activeAnimationStationaryOne;
IAnimation *activeAnimationStationaryTwo;

class PotPosLimits
{
public:
  PotPosLimits() {}

  PotPosLimits(uint16_t st, uint16_t sp)
  {
    start = st;
    stop = sp;
  }
  uint16_t start = 0;
  uint16_t stop = 0;
};

PotPosLimits potPositions[12];
IAnimation *ledFunctionsOne[12];
IAnimation *ledFunctionsWhenStationaryOne[12];

#ifdef LED_STRING_TWO_PRESENT
IAnimation *ledFunctionsWhenStationaryTwo[12];
IAnimation *ledFunctionsTwo[12];
#endif

uint16_t deltaOne = 1; // Sets forward or backwards direction amount. (Can be negative.) (delta = 1 WORKS)
#ifdef LED_STRING_TWO_PRESENT
uint16_t deltaTwo = 1; // Sets forward or backwards direction amount. (Can be negative.) (delta = 1 WORKS)
#endif

bool startUpDone = false;

struct ledData ledDataOne;
#ifdef LED_STRING_TWO_PRESENT
struct ledData ledDataTwo;
#endif

bool reverse = false; // the direction of the hall event

QList<bool> eventList; // has all interrupt events
bool lock = false;
byte potPin = A0;
uint16_t potVal = analogRead(A0);
bool hallSensorActive = true;
bool simulateHallSensorEvents = true;

#ifdef BEAT_ANIMATION_SUPPORTED

bool beatLock = false;
long previousTime = 0;
long currentTime = 0;
long deltaTime = 0;
long nextBeat = 0;
uint16_t averageBeatTime;
uint16_t beatStartedAgo;
long now = 0;
QList<uint16_t> beatEventList; // has all interrupt events
int lastBeatQueLength = 0;

void interruptBeat()
{
  if (beatLock)
    return;
  beatLock = true;

#ifdef SHOW_BEAT_INFO
  Serial.println("BEAT-----------------");
#endif

  if (startUpDone == false)
    return;

  // startup
  if (previousTime == 0)
  {
#ifdef SHOW_BEAT_INFO
    Serial.println("BEAT-------lock first time--");
#endif

    previousTime = millis();
  }
  else
  {

#ifdef SHOW_BEAT_INFO
    Serial.println("BEAT-------lock next time--");
#endif

    currentTime = millis();
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;
    eventList.push_back(deltaTime);
  }
  beatLock = false;
}
#endif

void interruptHall()
{
  if (startUpDone == false)
  {
    return;
  }
  hallSensorActive = true;
  eventList.push_back(true);

#ifdef SHOW_LOCK_AND_QUEUE_INFO
  Serial.println("------------------------------>");
  Serial.println(eventList.length());
  Serial.print("lock:");
  Serial.println(lock ? "true" : "false");
#endif
}
void interruptHallSimulated()
{
  eventList.push_back(true);
}

uint8_t potPosition = 0;

#ifdef BEAT_ANIMATION_SUPPORTED
void OnStationaryBeatAnimation()
{

#ifdef SHOW_BEAT_INFO
  Serial.println("BEAT-------FIRE-----");
#endif

  if (activeAnimationStationaryOne->IsBeatSupported())
  {
    activeAnimationStationaryOne->OnBeat(ledDataOne);
  }

#ifdef LED_STRING_TWO_PRESENT

  if (activeAnimationStationaryTwo->IsBeatSupported())
  {
    activeAnimationStationaryTwo->OnBeat(ledDataOne);
  }
#endif
}
#endif

void ledCodeOnHalEvent()
{
  if (simulateHallSensorEvents)
  {
    if (activeAnimationStationaryOne->IsBeatSupported())
      activeAnimationStationaryOne->OnBeat(ledDataOne);
    else
      activeAnimationStationaryOne->OnHallEvent(ledDataOne);

#ifdef LED_STRING_TWO_PRESENT
    if (activeAnimationStationaryTwo->IsBeatSupported())
      activeAnimationStationaryTwo->OnBeat(ledDataTwo);
    else
      activeAnimationStationaryTwo->OnHallEvent(ledDataTwo);
#endif
  }
  else
  {
    if (activeAnimationOne->IsBeatSupported())
      activeAnimationOne->OnBeat(ledDataOne);
    else
      activeAnimationOne->OnHallEvent(ledDataOne);

#ifdef LED_STRING_TWO_PRESENT
    if (activeAnimationTwo->IsBeatSupported())
      activeAnimationTwo->OnBeat(ledDataTwo);
    else
      activeAnimationTwo->OnHallEvent(ledDataTwo);
#endif
  }
}

void ledCodeOnLoop()
{
  activeAnimationOne->OnFastLoop(ledDataOne);

#ifdef LED_STRING_TWO_PRESENT
  activeAnimationTwo->OnFastLoop(ledDataTwo);
#endif
}

void ledCodeOnSetup()
{
  for (int i; i < numberOfAnimations; i++)
  {
    ledFunctionsOne[i]->OnSetup();
    ledFunctionsWhenStationaryOne[i]->OnSetup();
  }

#ifdef LED_STRING_TWO_PRESENT
  for (int i; i < numberOfAnimations; i++)
  {
    ledFunctionsTwo[i]->OnSetup();
    ledFunctionsWhenStationaryTwo[i]->OnSetup();
  }
#endif
}

void showStartupAnimation()
{
  ledsOne[0] = CRGB::Red;
#ifdef LED_STRING_TWO_PRESENT
  ledsTwo[0] = CRGB::Red;
#endif
  FastLED.show();
  Serial.println("red");

  delay(2000);
  ledsOne[1] = CRGB::Orange;
#ifdef LED_STRING_TWO_PRESENT
  ledsTwo[1] = CRGB::Orange;
#endif
  FastLED.show();
  Serial.println("orange");

  delay(2000);
  ledsOne[2] = CRGB::Yellow;
#ifdef LED_STRING_TWO_PRESENT
  ledsTwo[2] = CRGB::Yellow;
#endif
  FastLED.show();
  Serial.println("yellow");

  delay(2000);
  ledsOne[3] = CRGB::Green;
#ifdef LED_STRING_TWO_PRESENT
  ledsTwo[3] = CRGB::Green;
#endif
  Serial.println("green");
  FastLED.show();
  delay(2000);
  FastLED.clear();
}

void initPot()
{
  potPositions[0] = PotPosLimits(0, 50);
  potPositions[1] = PotPosLimits(51, 149);
  potPositions[2] = PotPosLimits(150, 200);
  potPositions[3] = PotPosLimits(201, 300);
  potPositions[4] = PotPosLimits(301, 400);
  potPositions[5] = PotPosLimits(401, 500);
  potPositions[6] = PotPosLimits(501, 600);
  potPositions[7] = PotPosLimits(601, 700);
  potPositions[8] = PotPosLimits(701, 800);
  potPositions[9] = PotPosLimits(801, 900);
  potPositions[11] = PotPosLimits(901, 1200);
}

#ifdef BEAT_ANIMATION_SUPPORTED

void CalculateBeat()
{
  now = millis();

  if (deltaTime > RESET_BEAT_AFTER_DELAY_TIME)
  {
    // RESET
#ifdef SHOW_BEAT_INFO
    Serial.println("BEAT-------RESET-----");
#endif

    eventList.clear();
    averageBeatTime = 0;
    beatStartedAgo = 0;
    lastBeatQueLength = 0;
    previousTime = 0;
    currentTime = 0;
  }
  else
  {
    int length = beatEventList.length();
    if (length >= 3 && length != lastBeatQueLength)
    {
      lastBeatQueLength = length;
      for (int i = 0; i < length; i++)
      {
        uint16_t delay = beatEventList.indexOf(i);
        averageBeatTime = (averageBeatTime + delay) / 2;
        beatStartedAgo += delay;
      }
      long beatStartTime = now - beatStartedAgo;
      nextBeat = beatStartTime + (beatStartedAgo / averageBeatTime) + averageBeatTime;
#ifdef SHOW_BEAT_INFO
      Serial.println("BEAT-------CALCULATED-----");
#endif
    }
    else
    {
#ifdef SHOW_BEAT_INFO
      // Serial.println("BEAT-------LIST TO SHORT-----");
#endif
    }
  }
}
#endif

void UpdatePotPosition()
{
  PotPosLimits limits;
  potVal = analogRead(potPin);
#ifdef SHOW_POTENTIOMETER_INFO
  Serial.print("potVal - read:");
  Serial.println(potVal);
#endif

  for (uint8_t i = 0; i < numberOfPotPositions; i++)
  {
    limits = potPositions[i];
    if (potVal >= limits.start && potVal < limits.stop)
    {
      potPosition = i;
      break;
    }
  }

  activeAnimationOne = ledFunctionsOne[potPosition];
  activeAnimationStationaryOne = ledFunctionsWhenStationaryOne[potPosition];

#ifdef LED_STRING_TWO_PRESENT
  activeAnimationTwo = ledFunctionsTwo[potPosition];
  activeAnimationStationaryTwo = ledFunctionsWhenStationaryTwo[potPosition];
#endif

#ifdef SHOW_POTENTIOMETER_INFO

  Serial.print("potVal:");
  Serial.println(potVal);
  Serial.print("pot Position:");
  Serial.println(potPosition);
  Serial.print("Limit start: ");
  Serial.println(limits.start);
  Serial.print("Limit stop: ");
  Serial.println(limits.stop);
  Serial.print("Active Animation One: ");
  Serial.println(activeAnimationOne->Name());

#endif
}
void UpdatePosition(struct ledData &data, uint16_t delta)
{
#ifdef SHOW_POSITION_PRINT_INFO
  Serial.print("pos one before:");
  Serial.println(data.pos);
#endif

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
#ifdef SHOW_POSITION_PRINT_INFO
  Serial.print("pos one after:");
  Serial.println(data.pos);
#endif
}

void loop()
{
#ifdef AUTO_FIRE_BEAT
  EVERY_N_MILLISECONDS(BEAT_SIMULATION_MS) // Advance pixels to next position.
  {
    OnStationaryBeatAnimation();
  }
#endif

  // slow updates here ...
  EVERY_N_MILLISECONDS(30000) // Advance pixels to next position.
  {

#ifdef AUTO_SELECT_ANIMATION
    if (potPosition < (numberOfAnimations - 1))
      potPosition++;
    else
      potPosition = 0;
#endif
  }

  // no need to read the Potentiometer often
  EVERY_N_MILLISECONDS(1000)
  {
#ifndef AUTO_SELECT_ANIMATION
    UpdatePotPosition();
#endif
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
#ifdef SIMULATED_INTERRUPT
    if (simulateHallSensorEvents)
      interruptHallSimulated();
#endif
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

      // que makes sure that we dont miss interrupt events
      eventList.pop_back();

      // digitalToggleFast(LED_BUILTIN);
    }
    lock = false;
  }

#ifdef BEAT_ANIMATION_SUPPORTED
  if (!beatLock)
  {
    beatLock = true;
    CalculateBeat();

    if (now > nextBeat)
    {
      nextBeat = now + averageBeatTime;
      OnStationaryBeatAnimation();
    }
    beatLock = false;
  }
#endif
}

void initAnimations()
{
  ledFunctionsOne[0] = new RotateAnimation(ColorMode::OneColor, CRGB::Blue, CRGB::Red, 2, 40, 12, NUM_LEDS_ONE);
  ledFunctionsOne[1] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 2, 40, 120, NUM_LEDS_ONE);
  ledFunctionsOne[2] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 2, 40, 120, NUM_LEDS_ONE);
  ledFunctionsOne[3] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 4, 10, 120, NUM_LEDS_ONE);
  ledFunctionsOne[4] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 4, 10, 120, NUM_LEDS_ONE);
  ledFunctionsOne[5] = new RotateAnimation(ColorMode::TwoColor, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  ledFunctionsOne[6] = new RotateAnimation(ColorMode::TwoColorFade, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  ledFunctionsOne[7] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  ledFunctionsOne[8] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  ledFunctionsOne[9] = new RainbowLedAnimation();
  ledFunctionsOne[10] = new JuggleAnimation();
  ledFunctionsOne[11] = new SinelonAnimation();

  ledFunctionsWhenStationaryOne[0] = new HeartBeatAnimation();
  ledFunctionsWhenStationaryOne[1] = new JuggleAnimation();
  ledFunctionsWhenStationaryOne[2] = new BpmAnimation();

  // ledFunctionsWhenStationaryOne[1] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 1, 40, 120, NUM_LEDS_ONE);
  ledFunctionsWhenStationaryOne[2] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 1, 40, 120, NUM_LEDS_ONE);
  ledFunctionsWhenStationaryOne[3] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 4, 10, 120, NUM_LEDS_ONE);
  ledFunctionsWhenStationaryOne[4] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 4, 10, 120, NUM_LEDS_ONE);
  ledFunctionsWhenStationaryOne[5] = new RotateAnimation(ColorMode::TwoColor, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  ledFunctionsWhenStationaryOne[6] = new RotateAnimation(ColorMode::TwoColorFade, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  ledFunctionsWhenStationaryOne[7] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  ledFunctionsWhenStationaryOne[8] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  ledFunctionsWhenStationaryOne[9] = new RainbowLedAnimation();
  ledFunctionsWhenStationaryOne[10] = new JuggleAnimation();
  ledFunctionsWhenStationaryOne[11] = new SinelonAnimation();

#ifdef LED_STRING_TWO_PRESENT
  ledFunctionsTwo[0] = new RotateAnimation(ColorMode::OneColor, CRGB::Blue, CRGB::Red, 1, 20, 12, NUM_LEDS_TWO);
  ledFunctionsTwo[1] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 1, 20, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[2] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 1, 20, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[3] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 2, 10, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[4] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 4, 5, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[5] = new RotateAnimation(ColorMode::TwoColor, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[6] = new RotateAnimation(ColorMode::TwoColorFade, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[7] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[8] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[9] = new RainbowLedAnimation();
  ledFunctionsTwo[10] = new JuggleAnimation();
  ledFunctionsTwo[11] = new SinelonAnimation();

  ledFunctionsWhenStationaryTwo[0] = new JuggleAnimation();
  //ledFunctionsWhenStationaryTwo[0] = new RotateAnimation(ColorMode::OneColor, CRGB::Blue, CRGB::Red, 1, 20, 12, NUM_LEDS_TWO);
  ledFunctionsWhenStationaryTwo[1] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 1, 20, 120, NUM_LEDS_TWO);
  ledFunctionsWhenStationaryTwo[2] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 1, 20, 120, NUM_LEDS_TWO);
  ledFunctionsWhenStationaryTwo[3] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 2, 10, 120, NUM_LEDS_TWO);
  ledFunctionsWhenStationaryTwo[4] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 4, 5, 120, NUM_LEDS_TWO);
  ledFunctionsWhenStationaryTwo[5] = new RotateAnimation(ColorMode::TwoColor, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  ledFunctionsWhenStationaryTwo[6] = new RotateAnimation(ColorMode::TwoColorFade, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  ledFunctionsWhenStationaryTwo[7] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  ledFunctionsWhenStationaryTwo[8] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  ledFunctionsWhenStationaryTwo[9] = new RainbowLedAnimation();
  ledFunctionsWhenStationaryTwo[10] = new JuggleAnimation();
  ledFunctionsWhenStationaryTwo[11] = new SinelonAnimation();
#endif
}

void setup()
{

  initAnimations();

  initPot();

  // numberOfAnimations = sizeof(ledFunctionsOne) / sizeof(ledFunctionsOne[0]);
  numberOfAnimations = 12;
  numberOfPotPositions = 12;

#ifndef AUTO_SELECT_ANIMATION
  UpdatePotPosition();
#endif

  // sizeof(potPosition) / sizeof(potPositions[0]); //results in 0

  ledDataOne = {ledsOne, 0, 0, NUM_LEDS_ONE};
#ifdef LED_STRING_TWO_PRESENT
  ledDataTwo = {ledsTwo, 0, 0, NUM_LEDS_TWO};
#endif

  Serial.begin(115200);
  Serial.println("Setup start --------------------------------------");
  Serial.print("Number of Pot Positions:");
  Serial.println(numberOfPotPositions);
  Serial.print("Number of Animations:");
  Serial.println(numberOfAnimations);

  FastLED.clear();
  delay(1600); // Startup delay

  pinMode(HALL_PIN, INPUT_PULLUP);
  pinMode(BEAT_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // setup Interrupt
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), interruptHall, FALLING);

#ifdef BEAT_ANIMATION_SUPPORTED // setup Interrupt
  attachInterrupt(digitalPinToInterrupt(BEAT_PIN), interruptBeat, FALLING);
#endif

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

  ledCodeOnSetup();

  Serial.println("6 -------------");

  Serial.print("NUM_LEDS_ONE: ");
  Serial.println(NUM_LEDS_ONE);
  Serial.print("no Of Leds: ");
  Serial.println(ledDataOne.noOfLeds);

  Serial.println("Setup done --------------------------------------");

  startUpDone = true;
}
