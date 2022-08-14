#ifndef APPLAUSE_HPP
#define APPLAUSE_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class ApplauseAnimation : public IAnimation
{
public:
    ApplauseAnimation() {}
    AnimationType Kind() { return AnimationType::OnHallEvent; }
    void OnBeat(struct ledData data){

        static uint16_t lastPixel = 0;
        fadeToBlackBy(data.leds, data.noOfLeds, 32);
        data.leds[lastPixel] = CHSV(random8(HUE_BLUE, HUE_PURPLE), 255, 255);
        lastPixel = random16(data.noOfLeds);
        data.leds[lastPixel] = CRGB::White;
    };
    String Name() { return "Applause"; }
};

#endif
