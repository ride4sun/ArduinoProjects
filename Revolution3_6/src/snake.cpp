#include <snake.h>

#define SNAKE_LENGTH 70
#define SNAKE_BACKGROUND_COLOR CRGB::Black

 uint8_t snakeHue = 0;

void snakeHallEvent(CRGB *leds, uint16_t pos, uint16_t lastPos)
{

    uint8_t oldHue = snakeHue;

    // paint background
    for (int i = 0; i < NUM_LEDS; i++)
    {

        leds[i] = SNAKE_BACKGROUND_COLOR;
    }

    if (pos > SNAKE_LENGTH)
    {
        // snake until the end
        for (int i = 0; i < SNAKE_LENGTH; i++)
        {
            leds[pos - i] = CHSV(snakeHue++, 255, 255);
        }
    }
    else
    {
        // loop for the few LEds on the beginning
        for (int i = pos; i >= 0; i--)
        {
            leds[i] = CHSV(snakeHue++, 255, 255);
        }
        // loop for the few LEds on the beginning
        for (int i = pos + NUM_LEDS - SNAKE_LENGTH; i < NUM_LEDS; i++)
        {
            leds[i] = CHSV(snakeHue++, 255, 255);
        }
    }
    snakeHue = oldHue + 10;
}
