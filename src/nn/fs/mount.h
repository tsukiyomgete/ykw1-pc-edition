#pragma once
#include <cstddef>
#include "nn/types.h"
#include "nn/account/uid.h"

namespace nn::fs
{
    Result QueryMountRomCacheSize(size_t* outSize);
    Result QueryMountAddOnContentCacheSize(size_t* outSize, int index);
    Result MountRom(const char* name, void* cache, size_t cacheSize);
    Result MountSaveData(const char* name, const nn::account::Uid& user);
    Result MountAddOnContent(const char* name, int index, void* cache, size_t cacheSize);
    void   Unmount(const char* name);
}
