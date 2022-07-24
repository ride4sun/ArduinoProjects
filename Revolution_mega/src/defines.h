#ifndef DEFINE_H
#define DEFINE_H


//2 wire LED setup - no clock chip
#define LED_TYPE WS2812B

// 4 Wire LED setup with clock chip
//#define LED_TYPE APA102
//#define CLK_PIN   13 // Don't need for WS2812b but necessary for APA102 (4 wire LED's)


#define HALL_PIN  3  // Hall Sensor Pin used for interrupt

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

#define NUMBER_OF_ANIMATIONS 5 //valid for inner and outer animation (always the same for now)
#define SIMULATED_INTERRUPT_TIME 20


struct ledData
{
  CRGB *leds;
  uint16_t pos;
  uint16_t lastPos;
  uint16_t noOfLeds;
};



class ILedAnimation
{
public:
   virtual void OnHallEvent(struct ledData) = 0;
   virtual void OnSetup() = 0;
   virtual void OnFastLoop() = 0;
};

#endif