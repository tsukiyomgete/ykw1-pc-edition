#pragma once
#include <cstdint>

namespace nn {


struct Result {
    uint32_t value = 0;
    bool IsSuccess() const { return value == 0; }
};


struct TimeSpan {
    int64_t nanoSeconds = 0;
    static TimeSpan FromMilliSeconds(int64_t ms) { return { ms * 1'000'000 }; }
};

}
