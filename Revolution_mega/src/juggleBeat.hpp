#ifndef JUGGLE_BEAT_HPP
#define JUGGLE_BEAT_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class JuggleBeatAnimation : public IAnimation
{
    uint8_t gHue = 0;   // rotating "base color" used by many of the patterns
    uint8_t bright = 0; // rotating "base color" used by many of the patterns
    bool toggleBright = false;

public:
    JuggleBeatAnimation() {}
    AnimationType Kind() { return AnimationType::OnBeatEvent; }

    void OnBeat(struct ledData data)
    {
        toggleBright = !toggleBright;
    }

    void OnFastLoop(struct ledData data)
    {
        // eight colored dots, weaving in and out of sync with each other
        fadeToBlackBy(data.leds, data.noOfLeds, 20);

        for (int i = 0; i < 8; i++)
        {
            data.leds[beatsin16(i + 7, 0, data.noOfLeds)] |= CHSV(gHue, 200, bright);
            gHue += 32;
        }
        if (toggleBright)
        {
            if (bright != 255)
                bright +=5;
        }
        else if (bright != 0)
        {
            bright-= 5;
        }
    }
    virtual uint8_t BeatPosIncrement() { return 1; }
    String Name() { return "Juggle Beat"; }
};

#endif
