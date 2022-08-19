#ifndef HEARTBEAT_HPP
#define HEARTBEAT_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class HeartBeatAnimation : public IAnimation
{
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t gHue = 0;
    uint8_t gHue1 = 0;
    bool toggle = false;

public:
    HeartBeatAnimation() {}
    AnimationType Kind() { return AnimationType::OnBeatEvent; }
    void OnBeat(struct ledData data)
    {
        fadeToBlackBy(data.leds, data.noOfLeds, data.pos);

        gHue1 = beatsin8(120, 0, 255);
        gHue = 0;

        for (uint16_t i = 0; i < data.noOfLeds; i++)
        {
            if (toggle)
            {
                data.leds[i] = CHSV(gHue1 + i + i, 255, 255);
            }
            else
            {
                data.leds[data.noOfLeds - i] = CHSV(gHue1 + i + i, 255, 255);
            }
        }

        toggle = !toggle;
    };

    uint8_t pos = 0;
    uint8_t pos2 = 0;

    void OnFastLoop(struct ledData data)
    {
        fadeToBlackBy(data.leds, data.noOfLeds, 25);
        pos = beatsin16(15, 0, data.noOfLeds - 1,0,0);
        pos2 = beatsin16(30, 0, data.noOfLeds - 1,0,180);

        data.leds[pos] += CHSV(gHue, 255, 192);
        data.leds[pos2] += CHSV(gHue, 255, 192);
    };

    String Name() { return "HeartBeat"; }
};

#endif
