#ifndef LEFTRIGHT_HPP
#define LEFTRIGHT_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class LeftRightAnimation : public IAnimation
{
public:
    LeftRightAnimation() {}
    bool toggle = false;
    AnimationType Kind() { return AnimationType::OnBeatEvent; }
    void OnBeat(struct ledData data)
    {
       fadeToBlackBy(data.leds, data.noOfLeds, 255);
        uint16_t half = data.noOfLeds / 2;  

        for (uint16_t i = 0; i < half; i++)
        {
            if (toggle)
            {
                data.leds[i] = CRGB::Red;
                data.leds[i+half] = CRGB::Black;
            }
            else
            {
                data.leds[i + half] = CRGB::Red;
                data.leds[i] = CRGB::Black;
            }
        }

        toggle = !toggle;
    };
    String Name() { return "LeftRight"; }
};

#endif
