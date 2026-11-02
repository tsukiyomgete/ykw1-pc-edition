#include "nn/fs/directory.h"
#include "nn/fs/detail.h"
#include "nn/fs/result.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>

namespace nn::fs {

//Ce que contient un DirectoryHandle : la liste lue a l'ouverture
struct DirectoryObject {
    std::vector<DirectoryEntry> entries;
    size_t next = 0;
};

Result CreateFile(const char* path, int64_t size)
{
    std::string realPath = detail::ResolvePath(path);
    if (realPath.empty())
    {
        return ResultPathNotFound;
    }
    std::error_code error;
    if (std::filesystem::exists(realPath, error))
    {
        return ResultPathAlreadyExists;
    }
    std::ofstream file(realPath, std::ios::binary);
    if (!file)
    {
        return ResultPathNotFound; //le dossier parent n'existe pas
    }
    file.close();
    std::filesystem::resize_file(realPath, (uintmax_t)size, error);
    return Result{};
}

Result DeleteFile(const char* path)
{
    std::string realPath = detail::ResolvePath(path);
    std::error_code error;
    if (realPath.empty() || !std::filesystem::is_regular_file(realPath, error))
    {
        return ResultPathNotFound;
    }
    std::filesystem::remove(realPath, error);
    return Result{};
}

Result RenameFile(const char* currentPath, const char* newPath)
{
    std::string from = detail::ResolvePath(currentPath);
    std::string to = detail::ResolvePath(newPath);
    std::error_code error;
    if (from.empty() || to.empty() || !std::filesystem::exists(from, error))
    {
        return ResultPathNotFound;
    }
    if (std::filesystem::exists(to, error))
    {
        return ResultPathAlreadyExists;
    }
    std::filesystem::rename(from, to, error);
    return Result{};
}

Result CreateDirectory(const char* path)
{
    std::string realPath = detail::ResolvePath(path);
    if (realPath.empty())
    {
        return ResultPathNotFound;
    }
    std::error_code error;
    if (std::filesystem::exists(realPath, error))
    {
        return ResultPathAlreadyExists;
    }
    if (!std::filesystem::create_directory(realPath, error))
    {
        return ResultPathNotFound;
    }
    return Result{};
}

Result GetEntryType(DirectoryEntryType* outType, const char* path)
{
    std::string realPath = detail::ResolvePath(path);
    std::error_code error;
    if (realPath.empty() || !std::filesystem::exists(realPath, error))
    {
        return ResultPathNotFound;
    }
    if (std::filesystem::is_directory(realPath, error))
    {
        *outType = DirectoryEntryType_Directory;
    }
    else
    {
        *outType = DirectoryEntryType_File;
    }
    return Result{};
}

Result OpenDirectory(DirectoryHandle* outHandle, const char* path, int mode)
{
    std::string realPath = detail::ResolvePath(path);
    std::error_code error;
    if (realPath.empty() || !std::filesystem::is_directory(realPath, error))
    {
        return ResultPathNotFound;
    }

    DirectoryObject* object = new DirectoryObject();
    for (const std::filesystem::directory_entry& item : std::filesystem::directory_iterator(realPath, error))
    {
        bool isDirectory = item.is_directory(error);
        //On ne garde que ce que le jeu a demande (dossiers, fichiers ou les deux)
        if (isDirectory && !(mode & OpenDirectoryMode_Directory))
        {
            continue;
        }
        if (!isDirectory && !(mode & OpenDirectoryMode_File))
        {
            continue;
        }

        DirectoryEntry entry = {};
        std::string name = item.path().filename().string();
        std::strncpy(entry.name, name.c_str(), sizeof(entry.name) - 1);
        if (isDirectory)
        {
            entry.type = DirectoryEntryType_Directory;
            entry.fileSize = 0;
        }
        else
        {
            entry.type = DirectoryEntryType_File;
            entry.fileSize = (int64_t)item.file_size(error);
        }
        object->entries.push_back(entry);
    }
    outHandle->handle = object;
    return Result{};
}

Result ReadDirectory(int64_t* outCount, DirectoryEntry* entries, DirectoryHandle handle, int64_t maxCount)
{
    DirectoryObject* object = (DirectoryObject*)handle.handle;
    int64_t count = 0;
    while (count < maxCount && object->next < object->entries.size())
    {
        entries[count] = object->entries[object->next];
        object->next++;
        count++;
    }
    *outCount = count;
    return Result{};
}

void CloseDirectory(DirectoryHandle handle)
{
    DirectoryObject* object = (DirectoryObject*)handle.handle;
    delete object;
}

}
