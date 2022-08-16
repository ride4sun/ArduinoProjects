#ifndef RAINBOWANIMATION_HPP
#define RAINBOWANIMATION_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class RainbowLedAnimation : public IAnimation
{

    uint8_t rainbowHue = 0;

public:
    RainbowLedAnimation() {}
    String Name() { return "Rainbow"; }
    AnimationType Kind() { return AnimationType::OnHallEvent; }
    void OnHall(struct ledData data)
    {
        for (uint16_t i = 0; i < data.noOfLeds; ++i)
        {
            data.leds[i] = CHSV(rainbowHue + (i), 255, 200);
        }
        rainbowHue += 20;
    };
    
};
#endif