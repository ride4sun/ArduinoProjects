#include <rainbow.h>

uint8_t rainbowHue = 0;

void rainbowHallEvent(CRGB *leds, uint16_t pos, uint16_t lastPos)
{
    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i] = CHSV(rainbowHue + (i * 10), 255, 255);
    }
    rainbowHue+=20;
}


void rainbowLoop()
{

}
