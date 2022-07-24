#ifndef BLUEANDWHITE_HPP
#define BLUEANDWHITE_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class BlueAndWhiteAnimation : public ILedAnimation
{
    uint16_t bw_spacing = 3;
    uint16_t bw_width = 1;

public:
    BlueAndWhiteAnimation() {}

    void OnHallEvent(struct ledData data){
        for (uint16_t i = 0; i < data.noOfLeds; i = (i + bw_spacing))
        {
            if ((i + data.pos) % 2)
            {
                data.leds[i] = CRGB::Black;
            }
            else
            {
                data.leds[i] = CRGB::Blue;
            }
        }
    };
    void OnSetup() {}
    void OnFastLoop() {}
};

#endif


