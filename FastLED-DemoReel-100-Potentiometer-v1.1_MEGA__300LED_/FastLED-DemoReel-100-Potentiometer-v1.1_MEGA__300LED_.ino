
//***************************************************************
// This is Mark Kriegsman's FastLED DemoReel100 example with
// a modificaiton to use a potentiometer for changing patterns.  The
// timer used for picking a new pattern has been commented out
// and there is a potentiometer to check inside the main loop now.
//
// You can view the serial monitor to see the Potentiameters Value hence with 'pattern' is running.
//
// 
// JonniJi - updated to use the Potentiometer to switch the patterns 04/05/20
//  
//***************************************************************

#include "FastLED.h"
#define DATA_PIN 2
#define potPin A0  // to read the potentiometer value 'A0')
//#define CLK_PIN   13
#define LED_TYPE  WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS   300
CRGB leds[NUM_LEDS];
#define BRIGHTNESS      128
#define FRAMES_PER_SECOND  120

uint16_t potVal ;  // initialize a variable to hold the potentiometers' value.
uint8_t mode ; // initialize a variable to hold which pattern is selected via the 'map' function. 

//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check that the serial monitor baud rate matches)
  delay(1500); // short delay for recovery
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);//works with WS2812B
//  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // use for leds with clockpin
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(); // clear out all LEDs.
  
//  Serial.println("Setup done.\n");
}

// Setup the list of patterns that can be ran.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
// below is an array of patterns/functions that can be run. Each is defined as a separate function below.
SimplePatternList gPatterns = { marquee_v3, candycane3, candycane4, twoDots, applause, fillAndCC, blinkyblink2, spew, confetti_GB, rainbow, confetti, sinelon, juggle };

uint8_t gCurrentPatternNumber = 0; // Initialize starting pattern value.
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

//---------------------------------------------------------------
void loop()
{
  //POT Stuff 
// EVERY_N_MILLISECONDS(500) {//  Don't Do this line? maybe it would work if I moved this all to a function?
  potVal = analogRead(potPin); // Pin to read the pot.
  mode = map(potVal, 0, 1023, 0, 12); //convert the pot range down - set total potential patterns to run that are listed in the SimplePatternList.
 Serial.print ("Mode: ") ;
 Serial.println (mode); // check to see what pattern number in the SimplePatternList array will run.. 

// delay (500); // Don't do this line. It screws up the pattern?  maybe it would work if this was all a function?
// }

 gCurrentPatternNumber = (mode); // john's line - use potVal (mode) to choose pattern to run.
//  gCurrentPatternNumber = 11; // used this line to manually change the pattern for testing.
  
  gPatterns[gCurrentPatternNumber](); // Call the current pattern function once, updating the 'leds' array
 
 //Serial.print (gCurrentPatternNumber); // used for testing which pattern is running. replaced with the (mode) print. 
  
  FastLED.show();  // send the 'leds' array out to the actual LED strip
  FastLED.delay(1000/FRAMES_PER_SECOND);  // insert a delay to keep the framerate modest

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // needed to slowly cycle the "base color" when using the 'rainbow' pattern..

}//end_main_loop

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


//---------------------------------------------------------------
//void nextPattern() // Don't need it for this version of the sketch. But maybe at it to a version somehow.?
//{
//   add one to the current pattern number, and wrap around at the end
//  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
//}

//////////////////////////
void marquee_v3() 
//revised version of marc millers - marquee_fun_v3 
//  Pixel position down the strip comes from this formula:
//      pos = spacing * (i-1) + spacing
//  i starts at 0 and is incremented by +1 up to NUM_LEDS/spacing.
//
// Marc Miller, May 2016
// Updated June 2018 - reordered some stuff and small bug fix.
{
uint16_t holdTime = 100;   // Milliseconds to hold position before advancing.
uint16_t spacing = 4;      // Sets pixel spacing. [Use 2 or greater] ( spacing = <3 WORKS good)
uint16_t delta = 1;         // Sets forward or backwards direction amount. (Can be negative.) (delta = 1 WORKS)
uint16_t width = 1;        // Can increase the number of pixels (width) of the chase. [1 or greater] (width = 1 WORKS)
uint8_t hue = 180;        // Starting color.
uint8_t hue2_shift = 20;  // Hue shift for secondary color.  Use 0 for no shift. [0-255]
int16_t pos;                // Pixel position.
int16_t advance = -1*width;  // Stores the advance amount.
uint8_t color;              // Stores a hue color.
long cycleCount = 0; // john added - used for printing how many times thru the complete loop cycle  

{ 
   FastLED.clear();  // clear all pixel data
 Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1600);  // Startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
    
  Serial.print("NUM_LEDS/spacing: ");  
  Serial.println(NUM_LEDS/spacing);
  Serial.println("Setup done. ");
    
    // Advance pixel postion down strip, and rollover if needed.
   // advance = (advance + delta + NUM_LEDS) % NUM_LEDS; // orginal code to advance forward or backward with -1
   advance = (advance + 1 + NUM_LEDS) % NUM_LEDS; // John modification - only advance forward '1'.
     
     {
        for (uint16_t i=0; i<(NUM_LEDS/spacing); i++){
        for (uint16_t w = 0; w < width; w++){
          
         pos = (spacing * (i-1) + spacing + advance + w - 1) % NUM_LEDS;
         leds[pos] = CRGB::Black; // Secondary Dot color
         // leds[pos] = CRGB::color2;
//          Serial.print(" "); Serial.print(pos); Serial.print("  ");
        }
//    Serial.println(" ");
      }
//    Serial.println(" ");
//   Serial.print (" Cycle Count = ");
//   Serial.println (cycleCount);
//    cycleCount = cycleCount + 1;
    }
    // Update pixels down the strip.
    for (uint16_t i=0; i<(NUM_LEDS/spacing); i++){
      for (uint16_t w = 0; w<width; w++){
        pos = (spacing * (i-1) + spacing + advance + w) % NUM_LEDS;
        if ( w % 2== 0 ){  // Is w even or odd? //\\error: want to clean out this code, but sketch will crash after a while.
          color = hue;                          // \\error: want to clean out this code, but sketch will crash after a while.
        } else {                                 // \\error: want to clean out this code, but sketch will crash after a while.
          color = hue + hue2_shift;             //  \\error: want to clean out this code, but sketch will crash after a while.
        }
      leds[pos] = CRGB::Blue; //Primary Dot color
       }
    }
    
    FastLED.show();

  }//end_every_n

}//end_main_loop 
 
/////////////////////////
void candycane3(){
//marc millers - candycane 
// "candy cane" stripes
// Original code concept by Christopher Kirkman
// Marc Miller, Dec 2017
// Add randomness for number of colors and length, Dec 2018
//
// modified by JonniJi to set 2 colors and set pixel length to '3' pixels per color (Apr 2020)
//
//
  
  CRGB color1 = CRGB::Blue;  // color used between color 2 (and 3 if used)
  CRGB color2 = CRGB::Red;
  
  const uint16_t travelSpeed = 200; // lower number is faster
  int shiftBy = -1;  //shiftBy can be positive or negative (to change direction)
  static uint16_t numColors = 2;  // Can be either 2 or 3
  static uint16_t stripeLength = 3;  //number of pixels per color
  
  static uint16_t qtyColor1 = 1;  // new values added by John (how many leds to light using color1)
  static uint16_t qtyColor2 = 3;  //new values added by john (how many leds to light using color2)
  
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
}//end candyCane3
/////////////////////////
void candycane4(){
  //marc millers - candycane 
// "candy cane" stripes
// Original code concept by Christopher Kirkman
// Marc Miller, Dec 2017
// Add randomness for number of colors and length, Dec 2018
//
// modified by JonniJi to set 2 colors and set pixel length to '4' per color (Apr 2020)
//
  
  CRGB color1 = CRGB::Blue;  // color used between color 2 (and 3 if used)
  CRGB color2 = CRGB::Red;
  
  const uint16_t travelSpeed = 200; // lower number is faster
  int shiftBy = -1;  //shiftBy can be positive or negative (to change direction)
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
}//end candyCane4

/////////////////////////
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 12);
}

//////////////////////////
void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), random8(128,200), random8(48,255));
}


//////////////////////////
void confetti_GB()
{
  // random colored speckles, Green and Blue hues only
  // Green = 96, Blue = 160
  uint8_t p = 25;  // What percentage of the time to make speckles.  [Range 0-100]

  fadeToBlackBy( leds, NUM_LEDS, 10);
  if (random8(100) < p) {
    int pos = random16(NUM_LEDS);
    uint8_t hue = random8(2);  // randomly chooses a 0 or 1
    if (hue == 0) {
      hue = random8(92,111);  // pick a hue somewhere in the green zone
    } else {
      hue = random8(156,165);  // pick a hue somewhere in the blue zone
    }
    leds[pos] += CHSV( hue, random8(200,240), 255);
  }
}//end confetti_GB


//////////////////////////
void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 12);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

//////////////////////////
void juggle() {
  // four colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 4; i++) {
    leds[beatsin16( i+5, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

//////////////////////////
void spew() {
  const uint16_t spewSpeed = 100;  // rate of advance
  static boolean spewing = 0;  // pixels are On(1) or Off(0)
  static uint8_t count = 1;  // how many to light (or not light)
  static uint8_t temp = count;
  static uint8_t hue = random8();
  EVERY_N_MILLISECONDS(spewSpeed){
    if (count == 0) {
      spewing = !spewing;
      if (spewing == 1) { count = random8(2,5); }  // random number for On pixels
      else { count = random8(1,8); }  // random number for Off pixels
      temp = count;
      //gHue = gHue - 30;
      hue = random8();
    }
    for (uint8_t i = NUM_LEDS-1; i > 0; i--) {
      leds[i] = leds[i-1];  // shift data down the line by one pixel
    }
    if (spewing == 1) {  // new pixels are On
      if (temp == count) {
        leds[0] = CHSV(hue-5, 215, 255);  // for first dot
        //leds[0] = CHSV(gHue-5, 215, 255);  // for first dot
      } else {
        leds[0] = CHSV(hue, 255, 255/(1+((temp-count)*2)) );  // for following dots
        //leds[0] = CHSV(gHue, 255, 255/(1+((temp-count)*2)) );  // for following dots
      }
    } else {  // new pixels are Off
      leds[0] = CHSV(0,0,0);  // set pixel 0 to black
    }
    count = count - 1;  // reduce count by one.
  }//end every_n
}//end spew

//////////////////////////  


//////////////////////////

void blinkyblink1() {
  static boolean dataIncoming = LOW;
  static boolean blinkGate1 = LOW;
  static boolean blinkGate2 = HIGH;
  static int8_t count = -1;

  EVERY_N_MILLISECONDS_I( timingObj, 250 ) {
    count++;
    if (count == 6) { count = 0; }
    blinkGate2 = count;
    dataIncoming = !dataIncoming;
    blinkGate1 = !blinkGate1;
    //Serial.print("c: "); Serial.print(count); Serial.print("\t");
    //Serial.print(dataIncoming); Serial.print("  "); Serial.print(blinkGate1);
    //Serial.print("\t"); Serial.print(dataIncoming * blinkGate1 * 255 * blinkGate2);
    //Serial.print("\tb: "); Serial.print(blinkGate2); Serial.println(" ");
    FastLED.clear();
    leds[0] = CHSV(gHue,0,dataIncoming*blinkGate1*255 * blinkGate2);
    if (count == 2 || count == 3) {
      timingObj.setPeriod( 50 );
    } else if (count == 4) {
      timingObj.setPeriod( 405 );
    } else {
      timingObj.setPeriod( 165 );
    }
  }
}//end_blinkyblink1

//////////////////////////
void blinkyblink2() {
  static boolean dataIncoming = LOW;
  static boolean blinkGate1 = LOW;
  static boolean blinkGate2 = HIGH;
  static int8_t count = -1;
  static int8_t P;

  EVERY_N_MILLISECONDS_I( timingObj, 250 ) {
    count++;
    if (count == 8) {
      count = 0;
      P = random8(NUM_LEDS);
    }
    blinkGate2 = count;
    dataIncoming = !dataIncoming;
    blinkGate1 = !blinkGate1;
    //Serial.print("c: "); Serial.print(count); Serial.print("\t");
    //Serial.print(dataIncoming); Serial.print("  "); Serial.print(blinkGate1);
    //Serial.print("\t"); Serial.print(dataIncoming * blinkGate1 * 255 * blinkGate2);
    //Serial.print("\tb: "); Serial.print(blinkGate2); Serial.println(" ");
    FastLED.clear();
    leds[P] = CHSV(gHue,255,dataIncoming*blinkGate1*255 * blinkGate2);
    if (count == 6) {
      timingObj.setPeriod( 250 );
    } else if (count == 7) {
      timingObj.setPeriod( 500 );
    } else {
      timingObj.setPeriod( 25 );
    }
  }
}//end_blinkyblink2

//////////////////////////
void fillAndCC() {
  static int16_t pos = 0;  // position along strip
  static int8_t delta = 3;  // delta (can be negative, and/or odd numbers)
  static uint8_t hue = 0;  // hue to display
  EVERY_N_MILLISECONDS(50) {
    leds[pos] = CHSV(hue,255,255);
    pos = (pos + delta + NUM_LEDS) % NUM_LEDS;
    if (delta >= 0 && pos == 0) {  //going forward
      hue = hue + random8(42,128);
    }
    if (delta < 0 && pos == NUM_LEDS-1) {  //going backward
      hue = hue + random8(42,128);
    }
  }
}//fillAndCC

//////////////////////////
void applause()    //originally by Mark K.
{
//Number of loops is increased based on number of pixels.
//Fade ammount drops as pixels increase.
static uint8_t appAmmount = (NUM_LEDS/32);
  for (uint8_t i=0; i<appAmmount; i++) {
    static uint16_t lastPixel = 0;
    fadeToBlackBy( leds, NUM_LEDS, 32/appAmmount );  //was 32
    leds[lastPixel] = CHSV(random8(HUE_BLUE,HUE_PURPLE),255,255);
    lastPixel = random16(NUM_LEDS);
    leds[lastPixel] = CRGB::White;
  }
}

//////////////////////////
void twoDots() {
  static uint8_t pos;  //used to keep track of position
  EVERY_N_MILLISECONDS(70) {
    fadeToBlackBy( leds, NUM_LEDS, 200);  //fade all the pixels some
    leds[pos] = CHSV(gHue, random8(170,230), 255);
    leds[(pos+5) % NUM_LEDS] = CHSV(gHue+64, random8(170,230), 255);
    pos = pos + 1;  //advance position
    
    //This following check is very important.  Do not go past the last pixel!
    if (pos == NUM_LEDS) { pos = 0; }  //reset to beginning
    //Trying to write data to non-existent pixels causes bad things.
  }
}//end_twoDots

//---------------------------------------------------------------

//---------Function to read the button and do something----------
//   This section could be changed to use a potentiometer,
//   encoder, or something else to trigger a pattern change.
//   Depending on what was used, the nextPattern() function
//   might need to be updated as well to have things work
//   the way you intend.

//void readbutton() {
//  myButton.read();
//  if(myButton.wasPressed()) {
 //   Serial.println("Button pressed!  Next pattern...   ");
//    nextPattern();  // Change to the next pattern


    //POT Stuff 
// void readPot() {
// EVERY_N_MILLISECONDS(500) {// 
// potVal = analogRead(potPin);
//  mode = map(potVal, 0, 675, 0, 12); //note: the potentiometer is messed up and doesn't go to 1023)
//  Serial.println (mode);
// delay (500);

//------------------- Program End ------------------
