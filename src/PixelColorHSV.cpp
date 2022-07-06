// Copyright (c) 2022 Dave Marsh. See LICENSE.

#include "PixelColorHSV.h"

namespace strip
{
    // void PixelColorHSV::fromRGB(PixelColor &color)
    // {
    //     const uint8_t cmax = findMax(color);
    //     const uint8_t cmin = findMin(color);
    //     const uint8_t delta = cmax - cmin;
    // }

    color_pack PixelColorHSV::ToRGB()
    {
        uint16_t mappedHue = mapHue();
        color_pack pack = mapHueToColor(mappedHue);
        pack = applySaturationValue(pack);
        return pack;
    }

} // namespace strip
