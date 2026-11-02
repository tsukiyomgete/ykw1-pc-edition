#pragma once
#include <cstdint>

namespace nn::util
{
    //Liste de N cases a cocher (comme nn::util::BitFlagSet de Nintendo)
    //Tag ne sert qu'a differencier les types (boutons, styles...)
    template <int N, typename Tag>
    struct BitFlagSet {
        uint32_t storage[(N + 31) / 32] = {};

        bool Test(int index) const
        {
            return (storage[index / 32] >> (index % 32)) & 1;
        }

        void Set(int index)
        {
            storage[index / 32] = storage[index / 32] | (1u << (index % 32));
        }

        void Unset(int index)
        {
            storage[index / 32] = storage[index / 32] & ~(1u << (index % 32));
        }

        void Reset()
        {
            for (int i = 0; i < (N + 31) / 32; i++)
            {
                storage[i] = 0;
            }
        }
    };
}
