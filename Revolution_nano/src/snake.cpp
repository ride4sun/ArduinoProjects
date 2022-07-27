#include <snake.h>

#define SNAKE_LENGTH 70
#define SNAKE_BACKGROUND_COLOR CRGB::Black

 uint8_t snakeHue = 0;

 void snakeHallEvent(struct ledData data)
 {

     Serial.println("snakeHallEvent---------------------------------------");
     Serial.println(data.pos);
     Serial.println(data.lastPos);
     Serial.println(data.noOfLeds);
     Serial.println("end snakeHallEvent---------------------------------------");

     uint8_t oldHue = snakeHue;

     // paint background
     for (uint16_t i = 0; i < data.noOfLeds; i++)
     {

         data.leds[i] = SNAKE_BACKGROUND_COLOR;
     }
     Serial.println("snake 1---------------------------------");

     if (data.pos > SNAKE_LENGTH)
     {
         Serial.println("snake 2---------------------------------");
         // snake until the end
         for (uint16_t i = 0; i < SNAKE_LENGTH; i++)
         {
             data.leds[data.pos - i] = CHSV(snakeHue++, 255, 255);
         }
     }
     else
     {
         Serial.println("snake 3---------------------------------");
         // loop for the few LEds on the beginning
         for (uint16_t i = data.pos; i >= 0; i--)
         {
             data.leds[i] = CHSV(snakeHue++, 255, 255);
         }
         Serial.println("snake 4---------------------------------");
         // loop for the few LEds on the beginning
         for (uint16_t i = data.pos + data.noOfLeds - SNAKE_LENGTH; i < data.noOfLeds; i++)
         {
             data.leds[i] = CHSV(snakeHue++, 255, 255);
         }
     }
     Serial.println("snake 5---------------------------------");
     snakeHue = oldHue + 10;
}
