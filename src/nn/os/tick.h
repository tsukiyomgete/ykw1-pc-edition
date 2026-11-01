#pragma once
#include <cstdint>
#include "nn/types.h"

namespace nn::os
{
    // --- Types
    struct Tick {
        int64_t value = 0;
    };

    // --- Tick (19.2 MHz clock, like the Switch)
    Tick GetSystemTick();
    int64_t GetSystemTickFrequency();
    TimeSpan ConvertToTimeSpan(Tick tick);
}
