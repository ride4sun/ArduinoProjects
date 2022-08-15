#ifndef DEFINE_H
#define DEFINE_H

// #define HALL_SUPPORTED
//#define POT_SUPPORTED

#define JACKET

//2 wire LED setup - no clock chip
#define LED_TYPE WS2812B

// 4 Wire LED setup with clock chip
//#define LED_TYPE APA102
//#define CLK_PIN   13 // Don't need for WS2812b but necessary for APA102 (4 wire LED's)

#define HALL_PIN  3  // Hall Sensor Pin used for interrupt

//BEAT

#define BEAT_PIN  2 // Beat Switch Pin used for interrupt
#define BEAT_SIMULATION_MS 1000
#define DEBOUNCE_SWITCH_MIN_MS 350
#define DEBOUNCE_SWITCH_MAX_MS 800
#define DEFAULT_SLOW_BEAT 3000
#define ACTIVE_BEAT_ANIMATION_DEFAULT 0
#define NO_OF_JACKET_BEAT_ANIMATION 3


#define MIN_NO_OF_BEATS 3
#define BEAT_MINUS_CORRECTION 5
//#define AUTO_FIRE_BEAT

#define RESET_BEAT_AFTER_DELAY_TIME  3000  // Reset the Beat detection after the user did not press the button for more than 3000

// #define NUM_LEDS_ONE 150 //testring
#define NUM_LEDS_ONE 48 //inner ring as one
// #define NUM_LEDS_ONE 144 //Jacket led
#define NUM_LEDS_TWO 52 //inner ring - same as on card

//#define NUM_LEDS_ONE 306 //big Ring on Golfcart


#define COLOR_ORDER GRB
#define DATA_PIN_ONE 5 // LED Signal Outer - Pin2 on the board.
//#define DATA_PIN_ONE 2 // LED Signal Outer - Pin2 on the board.
#define DATA_PIN_TWO 4 // LED Signal Inner - Pin4 on the board.

//comment out when there is only one led string present (ONE)
// #define LED_STRING_TWO_PRESENT 

#define BRIGHTNESS 100
 
#define SIMULATED_INTERRUPT_TIME 500
#define SIMULATED_INTERRUPT

//use when the potentiometer is not present
#define AUTO_SELECT_ANIMATION


//#define SHOW_POSITION_PRINT_INFO
//#define SHOW_POSITION_BEAT_PRINT_INFO
//#define SHOW_POTENTIOMETER_INFO
//#define SHOW_LOCK_AND_QUEUE_INFO
// #define SHOW_BEAT_INFO
//#define SHOW_STARTUP_ANIMATION




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
   virtual void Every20MilliSecond(struct ledData){}
   virtual void OnFastLoop(struct ledData){}

};


#define LONG_MAX 4294967295L

#endif