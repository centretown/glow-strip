#define UNITY_INCLUDE_PRINT_FORMATTED

#include <unity.h>
#include "base.h"
#include "config.h"
#include "wait.h"

using namespace pixel;

void SpinDevice(uint16_t index, color_pack color)
{
    Device &device = Pixels.device(index);
    Range range(device.Scope());

    range.Spin(device, color);
    device.Update();
}

void SpinDevices(color_pack color)
{
    for (uint8_t i = 0; i < Pixels.device_count(); i++)
    {
        SpinDevice(i, color);
    }
}

void testDevices()
{
    Color color(15, 0, 15);
    Color color1(0, 15, 15);
    Color color2(15, 15, 0);
    SpinDevices(color.Pack());
    wait(1000);
    SpinDevices(color1.Pack());
    wait(1000);
    SpinDevices(color2.Pack());
    wait(1000);
}


void testPixelDevices()
{
    RUN_TEST(testDevices);
}