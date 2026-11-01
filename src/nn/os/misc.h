#pragma once
#include <cstddef>
#include <cstdint>
#include "nn/types.h"

namespace nn::os
{
    // --- Types
    struct MemoryInfo {
        size_t totalAvailableMemorySize = 0;
        size_t totalUsedMemorySize = 0;
        size_t totalMemoryHeapSize = 0;
        size_t allocatedMemoryHeapSize = 0;
        size_t totalThreadStackSize = 0;
        int threadCount = 0;
    };

    // --- Misc
    void GenerateRandomBytes(void* buffer, size_t size);
    Result SetMemoryHeapSize(size_t size);
    Result AllocateMemoryBlock(uintptr_t* outAddress, size_t size);
    void QueryMemoryInfo(MemoryInfo* info);
}
