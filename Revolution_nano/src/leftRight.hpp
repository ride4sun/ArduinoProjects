#ifndef LEFTRIGHT_HPP
#define LEFTRIGHT_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class LeftRightAnimation : public IAnimation
{
public:
    LeftRightAnimation() {}

    uint8_t gHue = 0;

    bool toggle = false;
    AnimationType Kind() { return AnimationType::OnBeatEvent; }
    void OnBeat(struct ledData data)
    {
        fadeToBlackBy(data.leds, data.noOfLeds, data.pos);
        uint16_t half = data.noOfLeds / 2;
        gHue = beatsin8(120, 0, 255);

        for (uint16_t i = 0; i < half; i++)
        {
            if (toggle)
            {
                data.leds[i] = CHSV(gHue+i+i, 255, 255);

                data.leds[i + half] = CRGB::Black;
            }
            else
            {
                data.leds[i + half] = CHSV(gHue+i+i, 255, 255);

                data.leds[i] = CRGB::Black;
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
