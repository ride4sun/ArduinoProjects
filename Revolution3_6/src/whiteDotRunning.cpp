#include <whiteDotRunning.h>

void whiteDotEvent(CRGB *leds, uint16_t pos, uint16_t lastPos)
{
    leds[lastPos] = CRGB::Black;
    leds[pos] = CRGB::White;
}