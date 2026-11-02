#include "nn/os/misc.h"
#include <cstdlib>
#include <random>

namespace nn::os {

void GenerateRandomBytes(void* buffer, size_t size)
{
    std::random_device rng;                           
    uint8_t* bytes = static_cast<uint8_t*>(buffer);   
    for (size_t i = 0; i < size; i++)
    {
        bytes[i] = static_cast<uint8_t>(rng() % 256); 
    }
}

Result SetMemoryHeapSize(size_t size)
{
    (void)size;
    return Result{};
}

Result AllocateMemoryBlock(uintptr_t* outAddress, size_t size)
{
    void* block = malloc(size);
    *outAddress = reinterpret_cast<uintptr_t>(block); 
    return Result{};
}

void QueryMemoryInfo(MemoryInfo* info)
{
    *info = MemoryInfo{};   
}

}
