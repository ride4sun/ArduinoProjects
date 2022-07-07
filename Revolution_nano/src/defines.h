#pragma once
#define LED_TYPE WS2812B
//#define LED_TYPE APA102

#define CLK_PIN   7 // Don't need for WS2812b but necessary for APA102 (4 wire LED's)
#define HALL_PIN  3 // Hall Sensor Pin used for interrupt

#define NUM_LEDS_ONE 150 //testring
//#define NUM_LEDS_ONE 306 //big Ring on Cart
#define NUM_LEDS_TWO 44 //inner ring - same as on card

#define COLOR_ORDER GRB
#define DATA_PIN_ONE 2 // LED Signal Outer - Pin2 on the board.
#define DATA_PIN_TWO 4 // LED Signal Inner - Pin4 on the board.

#define BRIGHTNESS 100

#define NUMBER_OF_ANIMATIONS 5 //valid for inner and outer animation (always the same for now)

struct ledData
{
  CRGB *leds;
  uint16_t pos;
  uint16_t lastPos;
  uint16_t numberOfLeds;
} ;




