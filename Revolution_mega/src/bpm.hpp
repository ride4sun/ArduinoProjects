#ifndef BPM_HPP
#define BPM_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class BpmAnimation : public IAnimation
{
    bool toggle = false;
    uint8_t gHue = 0;
    bool second = false;
    bool delayed;
    CRGBPalette16 palette = PartyColors_p;

public:
    BpmAnimation(bool delayed = false)
    {
        this->delayed = delayed;
    }
    String Name() { return "BPM"; }
    AnimationType Kind() { return AnimationType::OnBeatEvent; }
    void OnBeat(struct ledData data)
    {
        if (delayed)
        {
            second = true;
            return;
        }
        else
        {
            second = true;
            fadeToBlackBy(data.leds, data.noOfLeds, data.pos);
            for (uint16_t i = 0; i < data.noOfLeds; i++)
            {
                data.leds[i] = palette;
                //data.leds[i] = CHSV(gHue, 255, 255);
            }
        }
    };

    void OnFastLoop(struct ledData data)
    {
        fadeToBlackBy(data.leds, data.noOfLeds, 25);
    };

    void Every100MilliSecond(struct ledData data)
    {
        if (second)
        {
            this->OnBeat(data);
        }
        second = false;
    };
};
#endif
