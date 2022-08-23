#ifndef RAINBOW2ANIMATION_HPP
#define RAINBOW2ANIMATION_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class Rainbow2LedAnimation : public IAnimation
{
    
    uint8_t rainbowHue = 0;

public:
    Rainbow2LedAnimation() {}
    AnimationType Kind() { return AnimationType::OnHallEvent; }
    void OnHall(struct ledData data)
    {
        fill_rainbow(data.leds, data.noOfLeds, rainbowHue);
        rainbowHue += 10;
    };
    void OnSetup(){}

    String Name() { return "Rainbow2"; }
};
#endif