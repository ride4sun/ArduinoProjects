#ifndef RAINBOWANIMATION_HPP
#define RAINBOWANIMATION_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class RainbowLedAnimation : public LedAnimation
{
    
    uint8_t rainbowHue = 0;

public:
    RainbowLedAnimation() {}
    void OnHallEvent(struct ledData data)
    {
        fill_rainbow(data.leds, data.noOfLeds, 7);
        rainbowHue += 10;
    };
    void OnSetup(){}
    void OnFastLoop() {}
};
#endif