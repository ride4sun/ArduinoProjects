#include <candyCane.h>

static uint16_t numColors = 2;    // Can be either 2 or 3
static uint16_t stripeLength = 6; // number of pixels per color
CRGB color1 = CRGB::Blue; // color used between color 2 (and 3 if used)
CRGB color2 = CRGB::Green;

void candyCaneHallEvent(struct ledData data)
{
    for (uint16_t i = 0; i < data.numberOfLeds; i++)
    {
        if ((i + data.pos) % ((numColors)*stripeLength) < stripeLength)
        {
            data.leds[i] = color2;
        }
        else
        {
            data.leds[i] = color1;
        }
    }
    // This is the section of code that makes the lights move
}
void candyCaneSetup() {}

void candyCaneLoop() {}