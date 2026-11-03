#pragma once
#include <cstddef>

namespace nn::lmem
{
    namespace detail
    {
        //Un "tas" : sur PC, on compte juste ce qui est alloue
        struct HeapHead {
            size_t size = 0;
            int allocationCount = 0;
        };
    }
    using HeapHandle = detail::HeapHead*;

    HeapHandle CreateExpHeap(void* address, size_t size, int option);
    void  DestroyExpHeap(HeapHandle heap);
    void* AllocateFromExpHeap(HeapHandle heap, size_t size);
    void  FreeToExpHeap(HeapHandle heap, void* address);
}
