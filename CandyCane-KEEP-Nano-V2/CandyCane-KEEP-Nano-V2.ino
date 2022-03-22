//***************************************************************
// "candy cane" stripes
// Original code concept by Christopher Kirkman
// https://plus.google.com/118127046082317810519/posts/WfEcTXiroke
//
// Modified to use EVERY_N timer, added option to travel in
// reverse, and added a third color option.
//
// Marc Miller, Dec 2017
// Add randomness for number of colors and length, Dec 2018
//***************************************************************

#include "FastLED.h"
//#include <OctoWS2811.h>  used with the OctoWS Board
//#define USE_OCTOWS2811

#define LED_TYPE WS2812B
#define DATA_PIN 2
//#define CLOCK_PIN 13 // Don't need clock pin with this LED lights
#define NUM_LEDS 150  //
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define BRIGHTNESS 150

//---------------------------------------------------------------
void setup() {
  FastLED.clear();  // clear all pixel data
  Serial.begin(9600);  // Allows serial monitor output (check baud rate)
  delay(1500);  // Startup delay
 //FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
void loop()
{
  candyCane();

}//end_main_loop


//---------------------------------------------------------------
//Draw alternating bands of color, 2 or 3 colors.
//When using three colors, color1 is used between the other two colors.
void candyCane(){
  CRGB color1 = CRGB::Blue;  // color used between color 2 (and 3 if used)
  CRGB color2 = CRGB::Red;
  
  const uint16_t travelSpeed = 200; // lower number is faster
  int shiftBy = 1;  //shiftBy can be positive or negative (to change direction)
  static uint16_t numColors = 2;  // Can be either 2 or 3
  static uint16_t stripeLength = 4;  //number of pixels per color
  
  static uint16_t qtyColor1 = 1;  // new values added by John (how many leds to light using color1)
  static uint16_t qtyColor2 = 4;  //new values added by john (how many leds to light using color2)
  
  static int offset;


  EVERY_N_MILLISECONDS(travelSpeed) {

  //Below is the jonniji modification to the loop.
      for (uint16_t i=0; i<NUM_LEDS; i++){
        if ( (i+offset)%((numColors)*stripeLength)<stripeLength ) {
          leds[i] = color2;
        } else {
          leds[i] = color1;
        }
      
      }

    FastLED.show();
// This is the section of code that makes the lights move

    offset = offset + shiftBy;
    if (shiftBy>0) {  //for positive shiftBy
     if (offset>=NUM_LEDS) offset = 0;
    } else {  //for negitive shiftBy
      if (offset<0) offset = NUM_LEDS;
    }

  }//end EVERY_N
}//end candyCane
