#ifndef DEFINE_H
#define DEFINE_H


//2 wire LED setup - no clock chip
#define LED_TYPE WS2812B

// 4 Wire LED setup with clock chip
//#define LED_TYPE APA102
//#define CLK_PIN   13 // Don't need for WS2812b but necessary for APA102 (4 wire LED's)

#define HALL_PIN  3  // Hall Sensor Pin used for interrupt

//BEAT
#define BEAT_ANIMATION_SUPPORTED
#define BEAT_PIN  5  // Beat Switch Pin used for interrupt
#define BEAT_SIMULATION_MS 1000
//#define AUTO_FIRE_BEAT

#define RESET_BEAT_AFTER_DELAY_TIME  3000  // Reset the Beat detection after the user did not press the button for more than 3000

#define NUM_LEDS_ONE 150 //testring
#define NUM_LEDS_TWO 52 //inner ring - same as on card

//#define NUM_LEDS_ONE 306 //big Ring on Golfcart


#define COLOR_ORDER GRB
#define DATA_PIN_ONE 2 // LED Signal Outer - Pin2 on the board.
//#define DATA_PIN_ONE 4 // LED Signal Outer - Pin2 on the board.
#define DATA_PIN_TWO 4 // LED Signal Inner - Pin4 on the board.

//comment out when there is only one led string present (ONE)
#define LED_STRING_TWO_PRESENT 

#define BRIGHTNESS 100
 


#define SIMULATED_INTERRUPT_TIME 500
#define SIMULATED_INTERRUPT


//use when the potentiometer is not present
//#define AUTO_SELECT_ANIMATION


//#define SHOW_POSITION_PRINT_INFO
//#define SHOW_POTENTIOMETER_INFO
//#define SHOW_LOCK_AND_QUEUE_INFO
#define SHOW_BEAT_INFO




#define HUE_WHITE 255

struct ledData
{
  CRGB *leds;
  uint16_t pos;
  uint16_t lastPos;
  uint16_t noOfLeds;
};



class IAnimation
{
public:
   virtual void OnHallEvent(struct ledData){};
   virtual void OnSetup(){};
   virtual void OnFastLoop(struct ledData){};
   virtual String Name() = 0;
   virtual bool IsBeatSupported(){return false;}
   virtual void OnBeat(struct ledData){};
};

#endif