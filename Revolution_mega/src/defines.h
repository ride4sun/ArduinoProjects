#ifndef DEFINE_H
#define DEFINE_H

//UNCOMMENT SECTION FOR JACKET
#define JACKET
#define NUM_LEDS_ONE 90 //Jacket led
//use when the potentiometer is not present
#define AUTO_SELECT_ANIMATION

//UNCOMMENT SECTION FOR GOLF CARD
//#define NUM_LEDS_ONE 306 //big Ring on Golfcart
//#define NUM_LEDS_TWO 48 //inner ring - same as on card
//#define LED_STRING_TWO_PRESENT 
//#define HALL_SUPPORTED
//#define POT_SUPPORTED

//UNCOMMENT SECTION FOR BIG TEST SETUP
//#define NUM_LEDS_ONE 150 //big Ring test setup
//#define NUM_LEDS_TWO 48 //inner ring - same as on card
//#define LED_STRING_TWO_PRESENT 
//#define HALL_SUPPORTED
//#define POT_SUPPORTED

//DEBUG INFORMATION
//#define SHOW_POSITION_PRINT_INFO
//#define SHOW_POSITION_BEAT_PRINT_INFO
//#define SHOW_POTENTIOMETER_INFO
//#define SHOW_LOCK_AND_QUEUE_INFO
//#define SHOW_BEAT_INFO
#define SHOW_STARTUP_ANIMATION

//2 wire LED setup - no clock chip
#define LED_TYPE WS2812B
#define FASTLED_INTERRUPT_RETRY_COUNT 2
// 4 Wire LED setup with clock chip
//#define LED_TYPE APA102
//#define CLK_PIN   13 // Don't need for WS2812b but necessary for APA102 (4 wire LED's)
#define HALL_PIN  3  // Hall Sensor Pin used for interrupt

//BEAT
#define BEAT_PIN  2 // Beat Switch Pin used for interrupt
#define BEAT_SIMULATION_MS 1000
#define DEBOUNCE_SWITCH_MIN_MS 350
#define DEBOUNCE_SWITCH_MAX_MS 1000
#define DEFAULT_SLOW_BEAT 3000
#define ACTIVE_BEAT_ANIMATION_DEFAULT 0
#define NO_OF_JACKET_BEAT_ANIMATION 6

#define SIMULATED_HALL_INTERRUPT_TIME 100
//#define SIMULATED_HALL_INTERRUPT


//BEAT SETUP - BEAT IS ALWAYS ENABLED
#define BRIGHTNESS 100
#define MIN_NO_OF_BEATS 4
#define BEAT_MINUS_CORRECTION 5
//#define AUTO_FIRE_BEAT
#define RESET_BEAT_AFTER_DELAY_TIME  3000  // Reset the Beat detection after the user did not press the button for more than 3000

//GENERAL SETUP
#define COLOR_ORDER GRB
#define DATA_PIN_ONE 4 // LED Signal Outer - Pin2 on the board.
//#define DATA_PIN_ONE 2 // LED Signal Outer - Pin2 on the board.
#define DATA_PIN_TWO 5 // LED Signal Inner - Pin4 on the board.

//THIS SECTION SHOULD NOT BE CHANGED AND IS A GENERAL INCLUDE ARE FOR TYPES
#define HUE_WHITE 255

struct ledData
{
  CRGB *leds;
  uint16_t pos;
  uint16_t lastPos;
  uint16_t noOfLeds;
};

enum AnimationType{
  OnHallEvent,
  OnBeatEvent,
  OnStaticEvent
};

enum ColorMode
{
    OneColor,
    TwoColor,
    OneColorFade,
    TwoColorFade,
    RainBow,
    RainBowFade,
};


enum BeatCalculationResult
{
  SameResultStillValid,
  NewResult,
  NotValidResult,
};

class IAnimation
{
public:
  virtual String Name() = 0;
  virtual AnimationType Kind() = 0;
  virtual void OnSetup(){}

   virtual void OnHall(struct ledData){}
   virtual void OnBeat(struct ledData){}
   virtual void OnStatic(struct ledData){}

   virtual uint8_t PosIncrement(){return 1;}
  //  virtual void Every20MilliSecond(struct ledData){}
   virtual void Every100MilliSecond(struct ledData){}
  //  virtual void Every200MilliSecond(struct ledData){}
   virtual void OnFastLoop(struct ledData){}

   virtual String ToString(){return "not defined";}

};

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

#define LONG_MAX 4294967295L

#endif