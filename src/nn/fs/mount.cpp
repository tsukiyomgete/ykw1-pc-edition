#include "nn/fs/mount.h"
#include "nn/fs/detail.h"
#include "nn/fs/result.h"
#include <cstdlib>
#include <map>
#include <mutex>
#include <string>

namespace nn::fs {

namespace detail {

// Where the Switch drives live on the PC (can be changed with YKW1_ROM_ROOT / YKW1_SAVE_ROOT)
static std::string FromEnv(const char* name, const char* fallback)
{
    const char* value = std::getenv(name);
    return (value != nullptr && value[0] != 0) ? value : fallback;
}

static const std::string ROM_ROOT  = FromEnv("YKW1_ROM_ROOT", "C:/Users/Tsukiyom/Documents/Modding/YKW1/romfs");
static const std::string SAVE_ROOT = FromEnv("YKW1_SAVE_ROOT", "save");

// Drive name ("rom", "save"...) -> folder on the PC
static std::map<std::string, std::string> g_Mounts;
static std::mutex g_MountsLock;   // the game loads files from several threads

const std::string& SaveRoot()
{
    return SAVE_ROOT;
}

std::string ResolvePath(const char* path)
{
    std::string full = path;
    size_t colon = full.find(':');
    if (colon == std::string::npos)
        return "";

    std::string drive = full.substr(0, colon);
    std::string rest  = full.substr(colon + 1);   // "/data/x.bin"
    if (rest.empty() || rest[0] != '/')
        rest = "/" + rest;

    std::lock_guard<std::mutex> guard(g_MountsLock);
    auto it = g_Mounts.find(drive);
    if (it == g_Mounts.end())
        return "";
    return it->second + rest;
}

static void Mount(const char* name, const std::string& folder)
{
    std::lock_guard<std::mutex> guard(g_MountsLock);
    g_Mounts[name] = folder;
}

static void Forget(const char* name)
{
    std::lock_guard<std::mutex> guard(g_MountsLock);
    g_Mounts.erase(name);
}

}

Result QueryMountRomCacheSize(size_t* outSize)
{
    *outSize = 0;   // no cache needed on PC
    return Result{};
}

Result QueryMountAddOnContentCacheSize(size_t* outSize, int index)
{
    (void)index;
    *outSize = 0;
    return Result{};
}

Result MountRom(const char* name, void* cache, size_t cacheSize)
{
    (void)cache; (void)cacheSize;
    detail::Mount(name, detail::ROM_ROOT);
    return Result{};
}

Result MountSaveData(const char* name, const nn::account::Uid& user)
{
    (void)user;   // one player on PC
    detail::Mount(name, detail::SAVE_ROOT);
    return Result{};
}

Result MountAddOnContent(const char* name, int index, void* cache, size_t cacheSize)
{
    // No additional content on PC
    (void)name; (void)index; (void)cache; (void)cacheSize;
    return ResultPathNotFound;
}

void Unmount(const char* name)
{
    detail::Forget(name);
}

}
