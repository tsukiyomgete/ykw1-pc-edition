#include "nn/os/tick.h"
#include <chrono>

namespace nn::os {

Tick GetSystemTick()
{
    std::chrono::steady_clock::duration now = std::chrono::steady_clock::now().time_since_epoch();
    int64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();

    Tick result;
    result.value = ns * 12 / 625;
    return result;
}

int64_t GetSystemTickFrequency()
{
    return 19200000;
}

TimeSpan ConvertToTimeSpan(Tick tick)
{
    TimeSpan result;
    result.nanoSeconds = (tick.value * 625)/12;
    return result;
}

}
