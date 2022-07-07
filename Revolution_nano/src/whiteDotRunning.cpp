#include <whiteDotRunning.h>

void whiteDotEvent(struct ledData data)
{
    data.leds[data.lastPos] = CRGB::Black;
    data.leds[data.pos] = CRGB::White;
}