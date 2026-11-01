#include "nn/os/misc.h"
#include <cstdlib>
#include <random>

namespace nn::os {

void GenerateRandomBytes(void* buffer, size_t size)
{
    std::random_device rng;                           // the PC's random source
    uint8_t* bytes = static_cast<uint8_t*>(buffer);   // see buffer as a list of bytes
    for (size_t i = 0; i < size; i++)
    {
        bytes[i] = static_cast<uint8_t>(rng() % 256); // keep only what fits in one byte
    }
}

Result SetMemoryHeapSize(size_t size)
{
    // On PC, memory is taken when needed: nothing to reserve.
    (void)size;
    return Result{};
}

Result AllocateMemoryBlock(uintptr_t* outAddress, size_t size)
{
    void* block = malloc(size);
    *outAddress = reinterpret_cast<uintptr_t>(block);  // give the address back as a number
    return Result{};
}

void QueryMemoryInfo(MemoryInfo* info)
{
    *info = MemoryInfo{};   // everything set to 0
}

}
