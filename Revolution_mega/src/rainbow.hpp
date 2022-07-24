#ifndef RAINBOWANIMATION_HPP
#define RAINBOWANIMATION_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class RainbowLedAnimation : public ILedAnimation
{

    uint8_t rainbowHue = 0;

public:
    RainbowLedAnimation() {}
    void OnHallEvent(struct ledData data)
    {
        for (uint16_t i = 0; i < data.noOfLeds; ++i)
        {
            data.leds[i] = CHSV(rainbowHue + (i * 10), 255, 255);
        }
        rainbowHue += 20;
    };
    void OnSetup() {}
    void OnFastLoop() {}
};
#endif