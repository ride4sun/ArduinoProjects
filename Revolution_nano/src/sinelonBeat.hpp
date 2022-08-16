#ifndef SINELON_BEAT_HPP
#define SINELON_BEAT_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class SinelonBeatAnimation : public IAnimation
{
    uint8_t gHue = 0;
    uint8_t pos = 0;
    uint8_t bright = 0; // rotating "base color" used by many of the patterns
    bool toggleBright = false;

public:
    SinelonBeatAnimation() {}
    AnimationType Kind() { return AnimationType::OnBeatEvent; }
    void OnBeat(struct ledData data)
    {
        // a colored dot sweeping back and forth, with fading trails
        fadeToBlackBy(data.leds, data.noOfLeds, 20);
        // alternative comment out to be red
        gHue = beatsin8(13, 0, data.noOfLeds - 1);

        toggleBright = !toggleBright;

    };


    void OnFastLoop(struct ledData data)
    {
        fadeToBlackBy(data.leds, data.noOfLeds, 120);
        pos = beatsin16(120, 0, data.noOfLeds - 1);
        data.leds[pos] += CHSV(gHue, 255, bright);

        if (toggleBright)
        {
            if (bright != 255)
                bright += 5;
        }
        else if (bright != 0)
        {
            bright -= 5;
        }
    };

    String Name() { return "Sinelon Beat"; }
};

#endif
