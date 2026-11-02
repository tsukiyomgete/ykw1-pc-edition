#pragma once
#include <cstdint>
#include "nn/types.h"

namespace nn::os
{
    // --- Types
    struct Tick {
        int64_t value = 0;
    };

    // --- Tick (horloge a 19,2 MHz comme la Switch)
    Tick GetSystemTick();
    int64_t GetSystemTickFrequency();
    TimeSpan ConvertToTimeSpan(Tick tick);
}
