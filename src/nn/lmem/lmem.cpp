#include "nn/lmem.h"
#include <cstdlib>

namespace nn::lmem {

HeapHandle CreateExpHeap(void* address, size_t size, int option)
{
    (void)address; //sur PC, malloc s'occupe de la memoire
    (void)option;
    detail::HeapHead* heap = new detail::HeapHead();
    heap->size = size;
    return heap;
}

void DestroyExpHeap(HeapHandle heap)
{
    delete heap;
}

void* AllocateFromExpHeap(HeapHandle heap, size_t size)
{
    void* block = std::malloc(size);
    if (block != nullptr)
    {
        heap->allocationCount++;
    }
    return block;
}

void FreeToExpHeap(HeapHandle heap, void* address)
{
    if (address == nullptr)
    {
        return;
    }
    std::free(address);
    heap->allocationCount--;
}

}
