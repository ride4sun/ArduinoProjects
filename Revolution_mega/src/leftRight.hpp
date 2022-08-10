#ifndef LEFTRIGHT_HPP
#define LEFTRIGHT_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class LeftRightAnimation : public IAnimation
{
public:
    LeftRightAnimation() {}

    void OnHallEvent(struct ledData data) {}
    void OnSetup() {}
    bool toggle = false;

    bool IsBeatSupported() { return true; }
    void OnBeat(struct ledData data)
    {
        fadeToBlackBy(data.leds, data.noOfLeds, data.noOfLeds);

        for (uint16_t i = 0; i < data.noOfLeds / 2; i++)
        {
            if (!toggle)
                data.leds[i] = CRGB::Red;
        }

        for (uint16_t i = data.noOfLeds / 2; i < data.noOfLeds; i++)
        {
            if (toggle)
                data.leds[i] = CRGB::Red;
        }
        toggle = !toggle;
    };
    String Name() { return "LeftRight"; }
};

#endif
