#ifndef ROTATE_HPP
#define ROTATE_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"


class RotateAnimation : public IAnimation
{
    uint16_t noOfSpots;
    uint16_t spotLength;
    uint8_t fadeRatio;
    uint16_t gap;
    CRGB colorOne;
    CRGB colorTwo;
    uint16_t numberOfLeds;
    uint8_t rainbowHue = 0;
    bool toggleColor = false;
    ColorMode colorMode;
    bool colorToggle;

public:
    RotateAnimation(ColorMode colorMode, CRGB colorOne, CRGB colorTwo, uint16_t noOfSpots, uint16_t spotLength, uint8_t fadeRatio, uint16_t numberOfLeds)
    {
        this->noOfSpots = noOfSpots;
        this->spotLength = spotLength;
        this->fadeRatio = fadeRatio;
        this->colorOne = colorOne;
        this->colorTwo = colorTwo;
        this->numberOfLeds = numberOfLeds;
        this->toggleColor = toggleColor;
        this->colorMode = colorMode;
        colorToggle = colorMode == TwoColorFade || colorMode == TwoColor;
    }

public:
    AnimationType Kind() { return AnimationType::OnHallEvent; }
    void OnHallEvent(struct ledData data)
    {
        if (colorMode == OneColorFade || colorMode == TwoColorFade || colorMode == RainBowFade)
            fadeToBlackBy(data.leds, data.noOfLeds, fadeRatio);
        else{
            FastLED.clear();
            // for (uint16_t b = 0; b < data.noOfLeds; b++)
            //     data.leds[b] = CRGB::Black;
        }
            
        uint16_t currentPos;
        uint16_t correctedPos;

        for (uint8_t s = 0; s < noOfSpots; s++)
        {
            currentPos = (data.pos + (s * gap)) % data.noOfLeds;

            for (uint16_t i = 0; i < spotLength; i++)
            {
                correctedPos  = (currentPos + i) % data.noOfLeds;

                if (colorMode == RainBow || colorMode == RainBowFade)
                {
                    data.leds[correctedPos] = CHSV(rainbowHue, 255, 255);
                }
                else
                {
                    if (toggleColor)
                        data.leds[correctedPos] = colorTwo;
                    else
                        data.leds[correctedPos] = colorOne;
                }
            }
            rainbowHue += 2;

            if (colorToggle)
                toggleColor = !toggleColor;
        }
    };
    void OnSetup()
    {

        Serial.println("-- -- -- -- --Rotate setup start-- -- -- -- -- -- -- --");
        this->gap = numberOfLeds / noOfSpots;
        Serial.print("noOfSpots: ");
        Serial.println(this->noOfSpots);
        Serial.print("spotLength: ");
        Serial.println(this->spotLength);
        Serial.print("fadeRatio: ");
        Serial.println(this->fadeRatio);
        Serial.print("colorOne: ");
        Serial.println(this->colorOne);
        Serial.print("colorTwo: ");
        Serial.println(this->colorTwo);
        Serial.print("gap: ");
        Serial.println(this->gap);
        Serial.print("Color Mode: ");
        Serial.println(this->colorMode);
        Serial.print("colorToggle: ");
        Serial.println(this->colorToggle);

        Serial.println("----------Rotate setup end----------------");
    }
    
    String Name() { return "Rotate"; }
};

#endif
