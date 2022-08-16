#include <Arduino.h>
#include <rotate.hpp>
#include <rotateBeat.hpp>
#include <leftRight.hpp>
#include <applause.hpp>
#include <heartBeat.hpp>
#include <sinelon.hpp>
#include <sinelonBeat.hpp>
#include <bpm.hpp>
#include <candyCane.hpp>
#include <rainbow2.hpp>
#include <rainbow.hpp>
#include <snake.hpp>
#include <whiteDotRunning.hpp>
#include <blueAndWhite.hpp>
#include <juggle.hpp>
#include <juggleBeat.hpp>
#include <SchedTask.h>
#include <Gaussian.h>
#include <LinkedList.h>

// #include <TaskScheduler.h>

#include "QList.h"
#include "defines.h"
#define FASTLED_INTERRUPT_RETRY_COUNT 2
// #define FASTLED_ALLOW_INTERRUPTS 0 // Needed to fix jitter/fluttering!
#include "FastLED.h"

CRGB ledsOne[NUM_LEDS_ONE]; // the led array representing the leds addressable by FASTLED
#ifdef LED_STRING_TWO_PRESENT
CRGB ledsTwo[NUM_LEDS_TWO]; // the led array representing the leds addressable by FASTLED
#endif

// We create the Scheduler that will be in charge of managing the tasks
// Scheduler runner;

bool beatTaskActive = false;

uint8_t numberOfAnimations = 0;
uint8_t numberOfPotPositions = 0;
uint8_t potPosition = 0;

IAnimation *activeAnimationOne;
IAnimation *activeAnimationTwo;

void BeatEvent();
void OnHall();

// setup task but make sure the event is not happening
// the time setup will be updated later to the beat
SchedTask beatTask(0, DEFAULT_SLOW_BEAT, BeatEvent);

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
#ifdef POT_SUPPORTED
PotPosLimits potPositions[12];
#endif

IAnimation *ledFunctionsOne[12];

#ifdef LED_STRING_TWO_PRESENT
IAnimation *ledFunctionsTwo[12];
#endif

bool startUpDone = false;

struct ledData ledDataOne;
#ifdef LED_STRING_TWO_PRESENT
struct ledData ledDataTwo;
#endif

bool reverse = false; // the direction of the hall event

#ifdef HALL_SUPPORTED
QList<bool> eventList; // has all interrupt events
bool lock = false;
bool hallSensorActive = true;
bool simulateHallSensorEvents = true;
#endif

// potentiometer values
byte potPin = A0;
uint16_t potVal = analogRead(A0);

// beat detection
bool beatLock = false;
bool disableBeatOnce = false;
long deltaTime = 0;
long nextBeat = 0;
uint16_t averageBeatTime;
uint16_t beatStartedAgo;
long now = 0;
LinkedList<uint16_t> beatEventList = LinkedList<uint16_t>();
// has all interrupt events
int lastBeatQueLength = 0;

long previousTime = 0;
long currentTime = 0;

// returns true if the pin read was valid
bool debounce(long currentTime, long previousTime)
{
  if (previousTime == 0)
  {
    // the first value is always valid
    return true;
  }
  else
  {
    if ((currentTime > (previousTime + DEBOUNCE_SWITCH_MIN_MS)))
    {
      // if (currentTime < (previousTime + DEBOUNCE_SWITCH_MAX_MS))
      return true;
      // else
      //   return false;
    }
    else
      return false;
  }
}

void interruptBeat()
{
  if (startUpDone == false)
    return;

  if (beatLock == true)
    return;
  beatLock = true;
  currentTime = millis();

  if (debounce(currentTime, previousTime) == true)
  {
    if (previousTime == 0)
    {
      previousTime = currentTime;
    }
    else
    {
      deltaTime = currentTime - previousTime;

      if (deltaTime > RESET_BEAT_AFTER_DELAY_TIME)
      {
        beatEventList.clear();
        lastBeatQueLength = 0;
        previousTime = 0;
        currentTime = 0;
#ifdef SHOW_BEAT_INFO
        Serial.println("BEAT-------beat measure reset");
#endif
      }
      else
      {
        beatEventList.add((uint16_t)deltaTime);
#ifdef SHOW_BEAT_INFO
        Serial.print("DELTA: ");
        Serial.println(deltaTime);
#endif
      }
      previousTime = currentTime;
    }
  }

  // beatLock = false;
}

#ifdef HALL_SUPPORTED

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
  OnHall();
}
#endif

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
  }

#ifdef LED_STRING_TWO_PRESENT
  for (int i; i < numberOfAnimations; i++)
  {
    ledFunctionsTwo[i]->OnSetup();
  }
#endif
}

void showStartupAnimation()
{

#ifdef SHOW_STARTUP_ANIMATION

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
#endif
}
#ifdef POT_SUPPORTED
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

  Serial.println("Init Potentiometer-------------------------");
}
#endif
#ifdef SHOW_BEAT_INFO
void printList()
{
  Serial.print("LIST ->");
  int length = beatEventList.size();
  for (int i = 0; i < length; i++)
  {
    uint16_t value = beatEventList[i];
    Serial.print(value);
    Serial.print(" ");
  }
  Serial.println(" ");
}
#endif

BeatCalculationResult CalculateBeat()
{
  now = millis();

  int length = beatEventList.size();
  if (length >= MIN_NO_OF_BEATS && length != lastBeatQueLength)
  {
#ifdef SHOW_BEAT_INFO
    printList();
#endif

    uint16_t highest = 0;
    uint16_t lowest = 10000;

    for (int i = 0; i < length; i++)
    {
      if (lowest > beatEventList[i])
      {
        lowest = beatEventList[i];
      }
      if (highest < beatEventList[i])
      {
        highest = beatEventList[i];
      }
    }
#ifdef SHOW_BEAT_INFO
    Serial.print("LOW: ");
    Serial.println(lowest);
    Serial.print("HIGH: ");
    Serial.println(highest);
#endif
    beatStartedAgo = beatEventList[0];
    averageBeatTime = beatEventList[0];
    lastBeatQueLength = length;

    // GaussianAverage average = GaussianAverage();

    for (int i = 1; i < length; i++)
    {
      uint16_t delay = beatEventList[i];

      if (delay > DEBOUNCE_SWITCH_MAX_MS)
        continue;

      if (delay == highest)
        continue;
      if (delay == lowest)
        continue;

      averageBeatTime = (averageBeatTime + delay) / 2;
      beatStartedAgo += delay;
    }

    averageBeatTime = averageBeatTime - BEAT_MINUS_CORRECTION;
    long beatStartTime = now - beatStartedAgo;
    nextBeat = beatStartTime + (beatStartedAgo / averageBeatTime) + averageBeatTime;

#ifdef SHOW_BEAT_INFO
    Serial.println("BEAT-------CALCULATED-----");
    Serial.print("Average:");
    Serial.println(averageBeatTime);
    Serial.print("Beat Start Time:");
    Serial.println(beatStartTime);
    Serial.print("Next Beat:");
    Serial.println(nextBeat);
    Serial.print("Beat Start ago:");
    Serial.println(beatStartedAgo);
#endif

#ifdef SHOW_BEAT_INFO
    Serial.println("BEAT-------NEW BEAT ACTIVATED-----");
#endif

    return BeatCalculationResult::NewResult;
  }
  else
  {
    if (length == lastBeatQueLength && length >= MIN_NO_OF_BEATS)
    {
      // Serial.println("Same result still valid in beat calculation!");
      return BeatCalculationResult::SameResultStillValid;
    }
    else
    {
      // Serial.println("Not valid result in beat calculation!");
      return BeatCalculationResult::NotValidResult;
    }
  }
}

#ifndef POT_SUPPORTED
void SelectFunction(uint8_t i)
{
  activeAnimationOne = ledFunctionsOne[i];
#ifdef LED_STRING_TWO_PRESENT
  activeAnimationTwo = ledFunctionsTwo[i];
#endif
}
#endif

#ifdef POT_SUPPORTED
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
#ifdef LED_STRING_TWO_PRESENT
  activeAnimationTwo = ledFunctionsTwo[potPosition];
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
#endif

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

void loopBeatHandling()
{
  if (!beatLock)
  {
    beatLock = true;

    switch (CalculateBeat())
    {
    case BeatCalculationResult::NewResult:
#ifdef SHOW_BEAT_INFO
      Serial.println("Set Beat to new Time---------------------------");
#endif
      beatTask.setPeriod(averageBeatTime);
      beatTaskActive = true;
      break;
    case BeatCalculationResult::SameResultStillValid:
      // do nothing - Beat is active
      break;
    case BeatCalculationResult::NotValidResult:
      if (beatTaskActive)
      {
#ifdef SHOW_BEAT_INFO
        Serial.println("Stop Beat Animation ---------------------------");
#endif
        beatTask.setPeriod(DEFAULT_SLOW_BEAT);
        beatTaskActive = false;
      }
      break;
    default:
      Serial.println("Unsupported Type of Beat Calculation detected !!!!!");
    }
  }
  beatLock = false;
}

#ifdef HALL_SUPPORTED
void OnHall()
{
  switch (activeAnimationOne->Kind())
  {
  case AnimationType::OnHallEvent:
    // Serial.print("On Hall Event ");
    // Serial.println(activeAnimationOne->Name());

    if (eventList.length() != 0)
    {
      // Serial.print("INNER: HALL --> ");
      // Serial.print(activeAnimationOne->Name());
      // Serial.print("Kind -> ");
      // Serial.print(activeAnimationOne->Kind());
      // Serial.print("Index -> ");
      // Serial.println(activeAnimationOne->ToString());
      // Serial.print("spotLength -> ");
      // Serial.println(RotateAnimation as activeAnimationOne)->getSpotLength());

      UpdatePosition(ledDataOne, activeAnimationOne->PosIncrement());
      activeAnimationOne->OnHall(ledDataOne);

#ifdef LED_STRING_TWO_PRESENT

      // Serial.print("INNER: HALL --> ");
      // Serial.print(activeAnimationTwo->Name());
      // Serial.print("Kind -> ");
      // Serial.print(activeAnimationTwo->Kind());
      // Serial.print("Index -> ");
      // Serial.println(activeAnimationTwo->ToString());

      UpdatePosition(ledDataTwo, activeAnimationTwo->PosIncrement());
      activeAnimationTwo->OnHall(ledDataTwo);
#endif
      // que makes sure that we dont miss interrupt events
      eventList.pop_back();
    }

    break;

  default:
    break;
  }

  FastLED.show();
}
#endif

void BeatEvent()
{
  switch (activeAnimationOne->Kind())
  {
  case AnimationType::OnBeatEvent:
    UpdatePosition(ledDataOne, activeAnimationOne->PosIncrement());
    activeAnimationOne->OnBeat(ledDataOne);

#ifdef LED_STRING_TWO_PRESENT
    UpdatePosition(ledDataTwo, activeAnimationTwo->PosIncrement());
    activeAnimationTwo->OnBeat(ledDataTwo);
#endif

    break;

  default:
    break;
    // Serial.print(activeAnimationOne->Kind());
    // Serial.print(" ");
    // Serial.print(activeAnimationOne->Name());
    // Serial.print(" ");
    // Serial.println("Unsupported Type of Animation detected on Beat Event !!!!!");
  }

  FastLED.show();
}

void OnStaticAnimationEvent()
{
  activeAnimationOne->OnStatic(ledDataOne);
#ifdef LED_STRING_TWO_PRESENT
  activeAnimationTwo->OnStatic(ledDataTwo);
#endif
  FastLED.show();
}

void loop()
{
  // runner.execute();
  SchedBase::dispatcher();

  // slow updates here ...
  EVERY_N_MILLISECONDS(30000) // Advance pixels to next position.
  {

#ifdef AUTO_SELECT_ANIMATION
    if (potPosition < (numberOfAnimations - 1))
    {
      potPosition++;
    }
    else
    {
      potPosition = 0;
    }

    activeAnimationOne = ledFunctionsOne[potPosition];
#ifdef LED_STRING_TWO_PRESENT
    activeAnimationTwo = ledFunctionsTwo[potPosition];
#endif

#endif
  }

  // no need to read the Potentiometer often
#ifdef POT_SUPPORTED
  EVERY_N_MILLISECONDS(1000)
  {
    UpdatePotPosition();
  }
#endif

#ifdef HALL_SUPPORTED
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
#endif

#ifdef HALL_SUPPORTED
  EVERY_N_MILLISECONDS(SIMULATED_HALL_INTERRUPT_TIME) // Advance pixels to next position.
  {
#ifdef SIMULATED_HALL_INTERRUPT
    if (simulateHallSensorEvents)
      interruptHallSimulated();
#endif
  }
#endif

#ifdef HALL_SUPPORTED
  if (lock == false)
  {
    // lock is making sure that the led routine gets not entered when it
    // is already running (avoid crash and reentrance)
    lock = true;
    OnHall();
  }
  lock = false;

#endif

  loopBeatHandling();
  ledCodeOnLoop();
  OnStaticAnimationEvent();
}

void initAnimations()
{
  Serial.println("Init Animations---------------------------");
#ifdef HALL_SUPPORTED
  ledFunctionsOne[0] = new RotateAnimation(0, ColorMode::OneColor, CRGB::Blue, CRGB::Red, 8, 4, 12, NUM_LEDS_ONE);
  ledFunctionsOne[1] = new RotateAnimation(1, ColorMode::TwoColor, CRGB::Blue, CRGB::Red, 8, 4, 120, NUM_LEDS_ONE);
  ledFunctionsOne[2] = new RotateAnimation(2, ColorMode::TwoColorFade, CRGB::Blue, CRGB::Red, 8, 4, 120, NUM_LEDS_ONE);
  ledFunctionsOne[3] = new RotateAnimation(3, ColorMode::RainBow, CRGB::Blue, CRGB::Red, 8, 4, 120, NUM_LEDS_ONE);
  ledFunctionsOne[4] = new RotateAnimation(4, ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 8, 4, 120, NUM_LEDS_ONE);
  ledFunctionsOne[5] = new JuggleAnimation();
  ledFunctionsOne[6] = new SinelonAnimation();
  ledFunctionsOne[7] = new RainbowLedAnimation();
  ledFunctionsOne[8] = new LeftRightAnimation();
  ledFunctionsOne[9] = new JuggleBeatAnimation();
  ledFunctionsOne[10] = new SinelonBeatAnimation();
  ledFunctionsOne[11] = new SinelonAnimation();

#ifdef LED_STRING_TWO_PRESENT
  ledFunctionsTwo[0] = new RotateAnimation(5, ColorMode::OneColor, CRGB::Blue, CRGB::Red, 4, 2, 12, NUM_LEDS_TWO);
  ledFunctionsTwo[1] = new RotateAnimation(6, ColorMode::TwoColor, CRGB::Blue, CRGB::Red, 4, 2, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[2] = new RotateAnimation(7, ColorMode::TwoColorFade, CRGB::Blue, CRGB::Red, 4, 2, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[3] = new RotateAnimation(8, ColorMode::RainBow, CRGB::Blue, CRGB::Red, 4, 2, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[4] = new RotateAnimation(8, ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 4, 2, 120, NUM_LEDS_TWO);
  ledFunctionsTwo[5] = new JuggleAnimation();
  ledFunctionsTwo[6] = new SinelonAnimation();
  ledFunctionsTwo[7] = new RainbowLedAnimation();
  ledFunctionsTwo[8] = new LeftRightAnimation();
  ledFunctionsTwo[9] = new JuggleBeatAnimation();
  ledFunctionsTwo[10] = new SinelonBeatAnimation();
  ledFunctionsTwo[11] = new SinelonAnimation();
#endif
#endif

#ifdef JACKET
  ledFunctionsOne[0] = new LeftRightAnimation();
  ledFunctionsOne[1] = new JuggleBeatAnimation();
  ledFunctionsOne[2] = new SinelonBeatAnimation();
  ledFunctionsOne[3] = new RotateBeatAnimation(ColorMode::OneColor, CRGB::Blue, CRGB::Red, 2, 2, 12, NUM_LEDS_ONE);
  ledFunctionsOne[4] = new BpmAnimation();
  ledFunctionsOne[5] = new LeftRightAnimation();
  ledFunctionsOne[6] = new LeftRightAnimation();
  ledFunctionsOne[7] = new JuggleAnimation();
  ledFunctionsOne[8] = new SinelonAnimation();
  ledFunctionsOne[9] = new SinelonAnimation();
  ledFunctionsOne[10] = new SinelonAnimation();
  ledFunctionsOne[11] = new SinelonAnimation();
#endif

  // ledFunctionsOne[5] = new RotateAnimation(ColorMode::TwoColor, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  // ledFunctionsOne[6] = new RotateAnimation(ColorMode::TwoColorFade, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  // ledFunctionsOne[7] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  // ledFunctionsOne[8] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);

  // ledFunctionsWhenStationaryOne[0] = new HeartBeatAnimation();
  // ledFunctionsWhenStationaryOne[1] = new LeftRightAnimation();
  // ledFunctionsWhenStationaryOne[2] = new JuggleAnimation();
  // ledFunctionsWhenStationaryOne[3] = new BpmAnimation();

  // ledFunctionsWhenStationaryOne[1] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 1, 40, 120, NUM_LEDS_ONE);
  // ledFunctionsWhenStationaryOne[2] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 1, 40, 120, NUM_LEDS_ONE);
  // ledFunctionsWhenStationaryOne[3] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 4, 10, 120, NUM_LEDS_ONE);
  // ledFunctionsWhenStationaryOne[4] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 4, 10, 120, NUM_LEDS_ONE);
  // ledFunctionsWhenStationaryOne[5] = new RotateAnimation(ColorMode::TwoColor, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  // ledFunctionsWhenStationaryOne[6] = new RotateAnimation(ColorMode::TwoColorFade, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  // ledFunctionsWhenStationaryOne[7] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  // ledFunctionsWhenStationaryOne[8] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 6, 8, 120, NUM_LEDS_ONE);
  // ledFunctionsWhenStationaryOne[9] = new RainbowLedAnimation();
  // ledFunctionsWhenStationaryOne[10] = new JuggleAnimation();
  // ledFunctionsWhenStationaryOne[11] = new SinelonAnimation();

  // ledFunctionsTwo[10] = new JuggleAnimation();
  // ledFunctionsTwo[11] = new SinelonAnimation();
  // ledFunctionsWhenStationaryTwo[0] = new JuggleAnimation();
  // ledFunctionsWhenStationaryTwo[1] = new ApplauseAnimation();
  // ledFunctionsWhenStationaryTwo[2] = new LeftRightAnimation();
  // ledFunctionsTwo[5] = new RotateAnimation(ColorMode::TwoColor, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  // ledFunctionsTwo[6] = new RotateAnimation(ColorMode::TwoColorFade, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  // ledFunctionsTwo[7] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  // ledFunctionsTwo[8] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  // ledFunctionsTwo[9] = new RainbowLedAnimation();
  // ledFunctionsWhenStationaryTwo[0] = new RotateAnimation(ColorMode::OneColor, CRGB::Blue, CRGB::Red, 1, 20, 12, NUM_LEDS_TWO);
  //  ledFunctionsWhenStationaryTwo[1] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 1, 20, 120, NUM_LEDS_TWO);
  //  ledFunctionsWhenStationaryTwo[2] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 1, 20, 120, NUM_LEDS_TWO);

  // ledFunctionsWhenStationaryTwo[3] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 2, 10, 120, NUM_LEDS_TWO);
  // ledFunctionsWhenStationaryTwo[4] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 4, 5, 120, NUM_LEDS_TWO);
  // ledFunctionsWhenStationaryTwo[5] = new RotateAnimation(ColorMode::TwoColor, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  // ledFunctionsWhenStationaryTwo[6] = new RotateAnimation(ColorMode::TwoColorFade, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  // ledFunctionsWhenStationaryTwo[7] = new RotateAnimation(ColorMode::RainBow, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  // ledFunctionsWhenStationaryTwo[8] = new RotateAnimation(ColorMode::RainBowFade, CRGB::Blue, CRGB::Red, 4, 6, 120, NUM_LEDS_TWO);
  // ledFunctionsWhenStationaryTwo[9] = new RainbowLedAnimation();
  // ledFunctionsWhenStationaryTwo[10] = new JuggleAnimation();
  // ledFunctionsWhenStationaryTwo[11] = new SinelonAnimation();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup start --------------------------------------");

  initAnimations();

#ifdef POT_SUPPORTED
  initPot();
#endif

// numberOfAnimations = sizeof(ledFunctionsOne) / sizeof(ledFunctionsOne[0]);
#ifdef JACKET
  numberOfAnimations = NO_OF_JACKET_BEAT_ANIMATION;
  numberOfPotPositions = 0;
#else
  numberOfAnimations = 12;
  numberOfPotPositions = 12;
#endif

#ifdef POT_SUPPORTED
  UpdatePotPosition();
#else
  SelectFunction(ACTIVE_BEAT_ANIMATION_DEFAULT);
#endif

  Serial.print("Number of Pot Positions:");
  Serial.println(numberOfPotPositions);
  Serial.print("Number of Animations:");
  Serial.println(numberOfAnimations);

#ifdef LED_STRING_TWO_PRESENT
  Serial.println("LED STRING TWO IS PRESENT");
#else
  Serial.println("LED STRING TWO IS NOT PRESENT");
#endif
#ifdef HALL_SUPPORTED
  Serial.println("HALL IS SUPPORTED");
#else
  Serial.println("HALL IS NOT SUPPORTED");
#endif
#ifdef JACKET
  Serial.println("JACKET IS SUPPORTED");
#else
  Serial.println("JACKET IS NOT SUPPORTED");
#endif
#ifdef POT_SUPPORTED
  Serial.println("POTENTIOMETER IS SUPPORTED");
#else
  Serial.println("POTENTIOMETER IS NOT SUPPORTED");
#endif

  // sizeof(potPosition) / sizeof(potPositions[0]); //results in 0

  ledDataOne = {ledsOne, 0, 0, NUM_LEDS_ONE};
#ifdef LED_STRING_TWO_PRESENT
  ledDataTwo = {ledsTwo, 0, 0, NUM_LEDS_TWO};
#endif

  FastLED.clear();
  delay(1600); // Startup delay

  // pinMode(LED_BUILTIN, OUTPUT);

#ifdef HALL_SUPPORTED
  pinMode(HALL_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), interruptHall, FALLING);
#endif

  pinMode(BEAT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BEAT_PIN), interruptBeat, RISING);

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
