#include <blueAndWhite.h>

uint8_t bw_spacing= 5;
uint8_t bw_width=3;

// set delta to 1 to work
// void blueAndWhiteEvent(CRGB *leds, uint16_t pos, uint16_t lastPos)
// {
//     for (int i = 0; i < NUM_LEDS; i = +(bw_spacing + bw_width))
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
void blueAndWhiteEvent(CRGB *leds, uint16_t pos, uint16_t lastPos)
{
    for (int i = 0; i < NUM_LEDS; i = (i + bw_spacing))
    {
        if ((i + pos) % 2)
        {
            leds[i] = CRGB::Black;
        }
        else
        {
            leds[i] = CRGB::Blue;
        }
  }
}
// set delta to 1 to work
// void blueAndWhiteEvent(CRGB *leds, uint16_t pos, uint16_t lastPos)
// {
//   for (int i = 0; i < NUM_LEDS; ++i)
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