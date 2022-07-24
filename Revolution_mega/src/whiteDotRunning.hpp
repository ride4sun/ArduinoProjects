#ifndef WHITEDOTRUNNING_HPP
#define WHITEDOTRUNNING_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class WhiteDotRunningAnimation : public ILedAnimation
{

public:
    WhiteDotRunningAnimation() {}

    void OnHallEvent(struct ledData data){
        data.leds[data.lastPos] = CRGB::Black;
        data.leds[data.pos] = CRGB::White;
    };
    void OnSetup() {}
    void OnFastLoop() {}
};

#endif
