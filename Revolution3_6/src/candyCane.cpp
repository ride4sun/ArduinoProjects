#include <candyCane.h>

static uint16_t numColors = 2;    // Can be either 2 or 3
static uint16_t stripeLength = 4; // number of pixels per color
CRGB color1 = CRGB::Blue; // color used between color 2 (and 3 if used)
CRGB color2 = CRGB::Black;


void candyCaneHallEvent(CRGB * leds, uint16_t pos, uint16_t lastPos)
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        if ((i + pos) % ((numColors)*stripeLength) < stripeLength)
        {
            leds[i] = color2;
        }
        else
        {
            leds[i] = color1;
        }
    }
    // This is the section of code that makes the lights move
}
void candyCaneSetup() {}

void candyCaneLoop() {}