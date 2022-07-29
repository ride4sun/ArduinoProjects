#ifndef ROTATE_HPP
#define ROTATE_HPP

#include <Arduino.h>
#include <FastLED.h>
#include "defines.h"

class RotateAnimation : public IAnimation
{
    uint16_t noOfSpots;
    uint16_t spotLength;
    uint16_t fadeLength;
    uint16_t gap;
    uint16_t colorOne;
    uint16_t colorTwo;
    uint16_t numberOfLeds;

public:
    RotateAnimation(uint16_t colorOne, uint16_t colorTwo, uint16_t noOfSpots, uint16_t spotLength, uint16_t fadeLength, uint16_t numberOfLeds)
    {

        this->noOfSpots = noOfSpots;
        this->spotLength = spotLength;
        this->fadeLength = fadeLength;
        this->colorOne = colorOne;
        this->colorTwo = colorTwo;
        this->numberOfLeds = numberOfLeds;
    }

public:
    void OnHallEvent(struct ledData data)
    {
        fadeToBlackBy(data.leds, data.noOfLeds, 50);
        // for (uint16_t i = 0; i < data.noOfLeds; i++)
        // {
        //     data.leds[i] = CRGB::Black;
        // }

        uint16_t currentPos;
        
        // data.leds[data.lastPos] = CRGB::Black;
        // data.leds[data.pos] = CRGB::White;
        for (uint16_t s = 0; s < noOfSpots; s++)
        {
            currentPos = data.pos + (gap * s);

            for (uint16_t i = 0; i < spotLength; i++)
            {
                currentPos = (currentPos + i) % data.noOfLeds;
                data.leds[currentPos] = CHSV(HUE_BLUE, 255, 255);
            }
        }

        // fadeToBlackBy(data.leds, numberOfLeds, 5);
    };
    void OnSetup()
    {

        Serial.println("-- -- -- -- --Rotate setup start-- -- -- -- -- -- -- --");
        this->gap = numberOfLeds / noOfSpots;
        Serial.println(this->noOfSpots);
        Serial.print("noOfSpots: ");
        Serial.println(this->noOfSpots);
        Serial.print("spotLength: ");
        Serial.println(this->spotLength);
        Serial.print("fadeLength: ");
        Serial.println(this->fadeLength);
        Serial.print("colorOne: ");
        Serial.println(this->colorOne);
        Serial.print("colorTwo: ");
        Serial.println(this->colorTwo);
        Serial.print("gap: ");
        Serial.println(this->gap);
        Serial.println("----------Rotate setup end----------------");
    }
    void OnFastLoop() {}
    String Name() { return "Rotate"; }
};

#endif
