#pragma once
#include <cstdint>

namespace nn::account
{
    struct Uid {
        uint64_t value[2] = {0, 0};
    };
}
