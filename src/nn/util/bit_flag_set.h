#pragma once
#include <cstdint>

namespace nn::util
{
    // A set of N on/off flags; Tag only gives the set its meaning (like nn::util::BitFlagSet)
    template <int N, typename Tag>
    struct BitFlagSet {
        uint32_t storage[(N + 31) / 32] = {};

        bool Test(int index) const { return (storage[index / 32] >> (index % 32)) & 1; }
        void Set(int index, bool on = true)
        {
            if (on) storage[index / 32] |=  (1u << (index % 32));
            else    storage[index / 32] &= ~(1u << (index % 32));
        }
        void Reset() { for (uint32_t& word : storage) word = 0; }
    };
}
