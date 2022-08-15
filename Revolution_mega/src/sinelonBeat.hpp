#ifndef SINELON_BEAT_HPP
#define SINELON_BEAT_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class SinelonBeatAnimation : public IAnimation
{
    uint8_t gHue = 0;
    uint8_t pos = 0;

public:
    SinelonBeatAnimation() {}
    AnimationType Kind() { return AnimationType::OnBeatEvent; }
    void OnBeat(struct ledData data)
    {
        // a colored dot sweeping back and forth, with fading trails
        fadeToBlackBy(data.leds, data.noOfLeds, 20);
        // alternative comment out to be red
        gHue = beatsin8(13, 0, data.noOfLeds - 1);
    };


    void OnFastLoop(struct ledData data)
    {
        fadeToBlackBy(data.leds, data.noOfLeds, 20);
        pos = beatsin16(120, 0, data.noOfLeds - 1);
        data.leds[pos] += CHSV(gHue, 255, 192);
    };

    String Name() { return "Sinelon Beat"; }
};

#endif
