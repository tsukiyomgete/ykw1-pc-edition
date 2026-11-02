#pragma once
#include "nn/hid/npad.h"

// Internal: the PC side of nn::hid (SDL2 gamepad + keyboard)
namespace nn::hid::detail
{
    void InitializeSdl();
    void ReadInput(NpadCommonState* outState);
    void Rumble(float low, float high);
}
