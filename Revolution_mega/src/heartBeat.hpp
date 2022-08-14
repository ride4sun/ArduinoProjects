#ifndef HEARTBEAT_HPP
#define HEARTBEAT_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class HeartBeatAnimation : public IAnimation
{
    uint8_t gHue = 0;
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;

public:
    HeartBeatAnimation() {}
    AnimationType Kind() { return AnimationType::OnBeatEvent; }
    void OnBeat(struct ledData data){
        
        uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
        // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
        for (uint16_t i = 0; i < data.noOfLeds; i++)
        { // 9948
            data.leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
        gHue++;

    };
    String Name() { return "HeartBeat"; }
};

#endif
