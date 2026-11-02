#include "nn/fs/mount.h"
#include "nn/fs/detail.h"

namespace nn::fs {

namespace detail {

std::string ResolvePath(const char* path)
{
    // TODO
    return path;
}

}

Result QueryMountRomCacheSize(size_t* outSize)
{
    // TODO
    (void)outSize;
    return Result{};
}

Result QueryMountAddOnContentCacheSize(size_t* outSize, int index)
{
    // TODO
    (void)outSize; (void)index;
    return Result{};
}

Result MountRom(const char* name, void* cache, size_t cacheSize)
{
    // TODO
    (void)name; (void)cache; (void)cacheSize;
    return Result{};
}

Result MountSaveData(const char* name, const nn::account::Uid& user)
{
    // TODO
    (void)name; (void)user;
    return Result{};
}

Result MountAddOnContent(const char* name, int index, void* cache, size_t cacheSize)
{
    // TODO
    (void)name; (void)index; (void)cache; (void)cacheSize;
    return Result{};
}

void Unmount(const char* name)
{
    // TODO
    (void)name;
}

}
