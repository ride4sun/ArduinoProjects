#ifndef LEFTRIGHTWIDE_HPP
#define LEFTRIGHTWIDE_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class LeftRightWideAnimation : public IAnimation
{
public:
    LeftRightWideAnimation() {}

    uint8_t gHue = 0;

    bool toggle = false;
    AnimationType Kind() { return AnimationType::OnBeatEvent; }
    void OnBeat(struct ledData data)
    {
        fadeToBlackBy(data.leds, data.noOfLeds, data.pos);
        
        gHue = beatsin8(120, 0, 255);

        for (uint16_t i = 0; i < data.noOfLeds; i++)
        {
            if (toggle)
            {
                data.leds[i] = CHSV(gHue+i+i, 255, 255);

            }
            else
            {
                data.leds[data.noOfLeds - i] = CHSV(gHue + i + i, 255, 255);
            }
        }

        toggle = !toggle;
    };

    uint8_t pos = 0;

    void OnFastLoop(struct ledData data)
    {
        fadeToBlackBy(data.leds, data.noOfLeds, 25);
        pos = beatsin16(13, 0, data.noOfLeds - 1);
        data.leds[pos] += CHSV(gHue, 255, 192);
    };

    String Name() { return "LeftRight"; }
};

#endif
