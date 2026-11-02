#pragma once
#include "nn/hid/npad.h"

//Interne : le cote PC de nn::hid (manette et clavier avec SDL2)
namespace nn::hid::detail
{
    void InitializeSdl();
    void ReadInput(NpadCommonState* outState);
    void Rumble(float low, float high);
}
