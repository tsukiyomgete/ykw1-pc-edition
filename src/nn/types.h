#pragma once
#include <cstdint>

namespace nn {

// Return value of most SDK functions: 0 = success.
struct Result {
    uint32_t value = 0;
    bool IsSuccess() const { return value == 0; }
};

// A duration, stored in nanoseconds.
struct TimeSpan {
    int64_t nanoSeconds = 0;
    static TimeSpan FromMilliSeconds(int64_t ms) { return { ms * 1000000 }; }
};

}