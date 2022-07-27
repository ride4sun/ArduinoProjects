#include <snake.h>

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void juggleHallEvent(struct ledData data)
{
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(data.leds, NUM_LEDS_ONE, 20);
    byte dothue = 0;
    for (int i = 0; i < 8; i++)
    {
        data.leds[beatsin16(i + 7, 0, NUM_LEDS_ONE - 1)] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}
