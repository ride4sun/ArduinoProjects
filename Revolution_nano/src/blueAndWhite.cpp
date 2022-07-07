#include <blueAndWhite.h>

uint8_t bw_spacing= 3;
uint8_t bw_width=1;

// set delta to 1 to work
// void blueAndWhiteEvent(CRGB *leds, uint16_t pos, uint16_t lastPos)
// {
//     for (int i = 0; i < NUM_LEDS_ONE; i = +(bw_spacing + bw_width))
//     {
//         if ((i + pos) % 2)
//         {
//             leds[i] = CRGB::Black;
//         }
//         else
//         {
//             leds[i] = CRGB::Blue;
//         }
//     }
// }
// set delta to 1 to work
void blueAndWhiteEvent(struct ledData data)
{
    for (uint16_t i = 0; i < data.numberOfLeds; i = (i + bw_spacing))
    {
        if ((i + data.pos) % 2)
        {
            data.leds[i] = CRGB::Black;
        }
        else
        {
            data.leds[i] = CRGB::Blue;
        }
  }
}
// set delta to 1 to work
// void blueAndWhiteEvent(CRGB *leds, uint16_t pos, uint16_t lastPos)
// {
//   for (int i = 0; i < NUM_LEDS_ONE; ++i)
//   {
//     if ((i + pos) % 2)
//     {
//       leds[i] = CRGB::Black;
//     }
//     else
//     {
//       leds[i] = CRGB::Blue;
//     }
//   }
// }
// set delta to 1 to work