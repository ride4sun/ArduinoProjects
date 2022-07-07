#include <snake.h>

#define SNAKE_LENGTH 70
#define SNAKE_BACKGROUND_COLOR CRGB::Black

 uint8_t snakeHue = 0;

 void snakeHallEvent(struct ledData data)
 {

     uint8_t oldHue = snakeHue;

     // paint background
     for (uint16_t i = 0; i < data.numberOfLeds; i++)
     {

         data.leds[i] = SNAKE_BACKGROUND_COLOR;
     }

     if (data.pos > SNAKE_LENGTH)
     {
         // snake until the end
         for (uint16_t i = 0; i < SNAKE_LENGTH; i++)
         {
             data.leds[data.pos - i] = CHSV(snakeHue++, 255, 255);
         }
     }
     else
     {
         // loop for the few LEds on the beginning
         for (uint16_t i = data.pos; i >= 0; i--)
         {
             data.leds[i] = CHSV(snakeHue++, 255, 255);
         }
         // loop for the few LEds on the beginning
         for (uint16_t i = data.pos + data.numberOfLeds - SNAKE_LENGTH; i < data.numberOfLeds; i++)
         {
             data.leds[i] = CHSV(snakeHue++, 255, 255);
         }
     }
     snakeHue = oldHue + 10;
}
