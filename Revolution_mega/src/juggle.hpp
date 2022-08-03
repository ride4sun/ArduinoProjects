#ifndef JUGGLE_HPP
#define JUGGLE_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class JuggleAnimation : public IAnimation
{
    uint8_t gHue = 0; // rotating "base color" used by many of the patterns

public:
    JuggleAnimation() {}
    void OnFastLoop(struct ledData data)
    {
        // eight colored dots, weaving in and out of sync with each other
        fadeToBlackBy(data.leds, data.noOfLeds, 20);

        for (int i = 0; i < 8; i++)
        {
            data.leds[beatsin16(i + 7, 0, data.noOfLeds)] |= CHSV(gHue, 200, 255);
            gHue += 32;
        }
    }
    String Name() { return "Juggle"; }
};

#endif
