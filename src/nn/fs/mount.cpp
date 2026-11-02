#include "nn/fs/mount.h"
#include "nn/fs/detail.h"
#include "nn/fs/result.h"
#include <cstdlib>
#include <map>
#include <mutex>
#include <string>

namespace nn::fs {

//Table des lecteurs : "rom" -> dossier sur le PC
static std::map<std::string, std::string> g_Mounts;
static std::mutex g_MountsLock;

//Dossier du RomFS (on peut le changer avec la variable YKW1_ROM_ROOT)
static std::string GetRomRoot()
{
    const char* value = std::getenv("YKW1_ROM_ROOT");
    if (value != nullptr && value[0] != 0)
    {
        return value;
    }
    return "C:/Users/Tsukiyom/Documents/Modding/YKW1/romfs";
}

//Dossier des sauvegardes (variable YKW1_SAVE_ROOT)
std::string detail::GetSaveRoot()
{
    const char* value = std::getenv("YKW1_SAVE_ROOT");
    if (value != nullptr && value[0] != 0)
    {
        return value;
    }
    return "save";
}

std::string detail::ResolvePath(const char* path)
{
    std::string full = path;
    size_t colon = full.find(':');
    if (colon == std::string::npos)
    {
        return "";
    }
    std::string drive = full.substr(0, colon); //"rom"
    std::string rest = full.substr(colon + 1); //"/data/x.bin"
    if (rest.empty() || rest[0] != '/')
    {
        rest = "/" + rest;
    }

    std::lock_guard<std::mutex> guard(g_MountsLock);
    if (g_Mounts.count(drive) == 0)
    {
        return ""; //lecteur pas monte
    }
    return g_Mounts[drive] + rest;
}

Result QueryMountRomCacheSize(size_t* outSize)
{
    *outSize = 0; //pas besoin de cache sur PC
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
    (void)cache;
    (void)cacheSize;
    std::lock_guard<std::mutex> guard(g_MountsLock);
    g_Mounts[name] = GetRomRoot();
    return Result{};
}

Result MountSaveData(const char* name, const nn::account::Uid& user)
{
    (void)user; //un seul joueur sur PC
    std::lock_guard<std::mutex> guard(g_MountsLock);
    g_Mounts[name] = detail::GetSaveRoot();
    return Result{};
}

Result MountAddOnContent(const char* name, int index, void* cache, size_t cacheSize)
{
    //Pas de contenu additionnel sur PC
    (void)name;
    (void)index;
    (void)cache;
    (void)cacheSize;
    return ResultPathNotFound;
}

void Unmount(const char* name)
{
    std::lock_guard<std::mutex> guard(g_MountsLock);
    g_Mounts.erase(name);
}

}
