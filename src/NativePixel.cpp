// Copyright (c) 2022 Dave Marsh. See LICENSE.
// This file copied part of the Adafruit_NeoPixel.cpp library.

#ifndef ARDUINO
#include "NativePixel.h"

namespace pixel
{
    const uint8_t NativePixel::_NeoPixelSineTable[256] = {
        128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170,
        173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211,
        213, 215, 218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240,
        241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254,
        254, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 251,
        250, 250, 249, 248, 246, 245, 244, 243, 241, 240, 238, 237, 235, 234, 232,
        230, 228, 226, 224, 222, 220, 218, 215, 213, 211, 208, 206, 203, 201, 198,
        196, 193, 190, 188, 185, 182, 179, 176, 173, 170, 167, 165, 162, 158, 155,
        152, 149, 146, 143, 140, 137, 134, 131, 128, 124, 121, 118, 115, 112, 109,
        106, 103, 100, 97, 93, 90, 88, 85, 82, 79, 76, 73, 70, 67, 65,
        62, 59, 57, 54, 52, 49, 47, 44, 42, 40, 37, 35, 33, 31, 29,
        27, 25, 23, 21, 20, 18, 17, 15, 14, 12, 11, 10, 9, 7, 6,
        5, 5, 4, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 11,
        12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35, 37,
        40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76,
        79, 82, 85, 88, 90, 93, 97, 100, 103, 106, 109, 112, 115, 118, 121,
        124};

    /* Similar to above, but for an 8-bit gamma-correction table.
       Copy & paste this snippet into a Python REPL to regenerate:
    import math
    gamma=2.6
    for x in range(256):
        print("{:3},".format(int(math.pow((x)/255.0,gamma)*255.0+0.5))),
        if x&15 == 15: print
    */
    const uint8_t NativePixel::_NeoPixelGammaTable[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3,
        3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6,
        6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10,
        11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17,
        17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
        25, 26, 27, 27, 28, 29, 29, 30, 31, 31, 32, 33, 34, 34, 35,
        36, 37, 38, 38, 39, 40, 41, 42, 42, 43, 44, 45, 46, 47, 48,
        49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
        64, 65, 66, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 80, 81,
        82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96, 97, 99, 100, 102,
        103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125,
        127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152,
        154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182,
        184, 186, 188, 191, 193, 195, 197, 199, 202, 204, 206, 209, 211, 213, 215,
        218, 220, 223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252,
        255};

    // A 32 - bit variant of gamma8() that applies the same function
    // to all components of a packed RGB or WRGB value.
    uint32_t NativePixel::gamma32(uint32_t x)
    {
        uint8_t *y = (uint8_t *)&x;
        // All four bytes of a 32-bit value are filtered even if RGB (not WRGB),
        // to avoid a bunch of shifting and masking that would be necessary for
        // properly handling different endianisms (and each byte is a fairly
        // trivial operation, so it might not even be wasting cycles vs a check
        // and branch for the RGB case). In theory this might cause trouble *if*
        // someone's storing information in the unused most significant byte
        // of an RGB value, but this seems exceedingly rare and if it's
        // encountered in reality they can mask values going in or coming out.
        for (uint8_t i = 0; i < 4; i++)
            y[i] = gamma8(y[i]);
        return x; // Packed 32-bit return
    }

    /*!
      @brief   Convert hue, saturation and value into a packed 32-bit RGB color
               that can be passed to setPixelColor() or other RGB-compatible
               functions.
      @param   hue  An unsigned 16-bit value, 0 to 65535, representing one full
                    loop of the color wheel, which allows 16-bit hues to "roll
                    over" while still doing the expected thing (and allowing
                    more precision than the wheel() function that was common to
                    prior NeoPixel examples).
      @param   sat  Saturation, 8-bit value, 0 (min or pure grayscale) to 255
                    (max or pure hue). Default of 255 if unspecified.
      @param   val  Value (brightness), 8-bit value, 0 (min / black / off) to
                    255 (max or full brightness). Default of 255 if unspecified.
      @return  Packed 32-bit RGB with the most significant byte set to 0 -- the
               white element of WRGB pixels is NOT utilized. Result is linearly
               but not perceptually correct, so you may want to pass the result
               through the gamma32() function (or your own gamma-correction
               operation) else colors may appear washed out. This is not done
               automatically by this function because coders may desire a more
               refined gamma-correction function than the simplified
               one-size-fits-all operation of gamma32(). Diffusing the LEDs also
               really seems to help when using low-saturation colors.
    */

    uint32_t NativePixel::ColorHSV(uint16_t hue, uint8_t sat, uint8_t val)
    {

        uint8_t r, g, b;

        // Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
        // 0 is not the start of pure red, but the midpoint...a few values above
        // zero and a few below 65536 all yield pure red (similarly, 32768 is the
        // midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
        // each for red, green, blue) really only allows for 1530 distinct hues
        // (not 1536, more on that below), but the full unsigned 16-bit type was
        // chosen for hue so that one's code can easily handle a contiguous color
        // wheel by allowing hue to roll over in either direction.
        hue = (hue * 1530L + 32768) / 65536;
        // Because red is centered on the rollover point (the +32768 above,
        // essentially a fixed-point +0.5), the above actually yields 0 to 1530,
        // where 0 and 1530 would yield the same thing. Rather than apply a
        // costly modulo operator, 1530 is handled as a special case below.

        // So you'd think that the color "hexcone" (the thing that ramps from
        // pure red, to pure yellow, to pure green and so forth back to red,
        // yielding six slices), and with each color component having 256
        // possible values (0-255), might have 1536 possible items (6*256),
        // but in reality there's 1530. This is because the last element in
        // each 256-element slice is equal to the first element of the next
        // slice, and keeping those in there this would create small
        // discontinuities in the color wheel. So the last element of each
        // slice is dropped...we regard only elements 0-254, with item 255
        // being picked up as element 0 of the next slice. Like this:
        // Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
        // Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
        // Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
        // and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
        // the constants below are not the multiples of 256 you might expect.

        // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
        if (hue < 510)
        { // Red to Green-1
            b = 0;
            if (hue < 255)
            { //   Red to Yellow-1
                r = 255;
                g = hue; //     g = 0 to 254
            }
            else
            {                  //   Yellow to Green-1
                r = 510 - hue; //     r = 255 to 1
                g = 255;
            }
        }
        else if (hue < 1020)
        { // Green to Blue-1
            r = 0;
            if (hue < 765)
            { //   Green to Cyan-1
                g = 255;
                b = hue - 510; //     b = 0 to 254
            }
            else
            {                   //   Cyan to Blue-1
                g = 1020 - hue; //     g = 255 to 1
                b = 255;
            }
        }
        else if (hue < 1530)
        { // Blue to Red-1
            g = 0;
            if (hue < 1275)
            {                   //   Blue to Magenta-1
                r = hue - 1020; //     r = 0 to 254
                b = 255;
            }
            else
            { //   Magenta to Red-1
                r = 255;
                b = 1530 - hue; //     b = 255 to 1
            }
        }
        else
        { // Last 0.5 Red (quicker than % operator)
            r = 255;
            g = b = 0;
        }

        // Apply saturation and value to R,G,B, pack into 32-bit result:
        uint32_t v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
        uint16_t s1 = 1 + sat;  // 1 to 256; same reason
        uint8_t s2 = 255 - sat; // 255 to 0
        return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
               (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
               (((((b * s1) >> 8) + s2) * v1) >> 8);
    }
}
#endif
