#ifndef BPM_HPP
#define BPM_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class BpmAnimation : public IAnimation
{
    bool toggle = false;
    uint8_t gHue = 0;

public:
    BpmAnimation() {}
    String Name() { return "BPM"; }
    AnimationType Kind() { return AnimationType::OnBeatEvent; }
    void OnBeat(struct ledData data)
    {
        // fadeToBlackBy(data.leds, data.noOfLeds, 150);
        

        for (uint16_t i = 0; i < data.noOfLeds; i++)
        { // 9948
            uint8_t beat = beatsin8(i, 0, 30);
            if (toggle)
                data.leds[i] = CHSV(beat, 255, 255);
            else
                data.leds[i] = CRGB::Black;
        }
        gHue++;
        toggle = !toggle;
        // gHue++;
    };
};
#endif
