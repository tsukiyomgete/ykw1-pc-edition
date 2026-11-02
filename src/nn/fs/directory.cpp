#include "nn/fs/directory.h"
#include "nn/fs/detail.h"
#include "nn/fs/result.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>

namespace fsys = std::filesystem;

namespace nn::fs {

namespace detail {

// What a DirectoryHandle points to: the entries, read once at OpenDirectory
struct DirectoryObject {
    std::vector<DirectoryEntry> entries;
    size_t next = 0;
};

}

Result CreateFile(const char* path, int64_t size)
{
    std::string realPath = detail::ResolvePath(path);
    if (realPath.empty())
        return ResultPathNotFound;
    std::error_code error;
    if (fsys::exists(realPath, error))
        return ResultPathAlreadyExists;
    {
        std::ofstream file(realPath, std::ios::binary);
        if (!file)
            return ResultPathNotFound;   // the parent folder does not exist
    }
    fsys::resize_file(realPath, static_cast<uintmax_t>(size), error);
    return Result{};
}

Result DeleteFile(const char* path)
{
    std::string realPath = detail::ResolvePath(path);
    std::error_code error;
    if (realPath.empty() || !fsys::is_regular_file(realPath, error))
        return ResultPathNotFound;
    fsys::remove(realPath, error);
    return Result{};
}

Result RenameFile(const char* currentPath, const char* newPath)
{
    std::string from = detail::ResolvePath(currentPath);
    std::string to   = detail::ResolvePath(newPath);
    std::error_code error;
    if (from.empty() || to.empty() || !fsys::exists(from, error))
        return ResultPathNotFound;
    if (fsys::exists(to, error))
        return ResultPathAlreadyExists;
    fsys::rename(from, to, error);
    return Result{};
}

Result CreateDirectory(const char* path)
{
    std::string realPath = detail::ResolvePath(path);
    if (realPath.empty())
        return ResultPathNotFound;
    std::error_code error;
    if (fsys::exists(realPath, error))
        return ResultPathAlreadyExists;
    if (!fsys::create_directory(realPath, error))
        return ResultPathNotFound;
    return Result{};
}

Result GetEntryType(DirectoryEntryType* outType, const char* path)
{
    std::string realPath = detail::ResolvePath(path);
    std::error_code error;
    if (realPath.empty() || !fsys::exists(realPath, error))
        return ResultPathNotFound;
    *outType = fsys::is_directory(realPath, error) ? DirectoryEntryType_Directory : DirectoryEntryType_File;
    return Result{};
}

Result OpenDirectory(DirectoryHandle* outHandle, const char* path, int mode)
{
    std::string realPath = detail::ResolvePath(path);
    std::error_code error;
    if (realPath.empty() || !fsys::is_directory(realPath, error))
        return ResultPathNotFound;

    detail::DirectoryObject* object = new detail::DirectoryObject();
    for (const fsys::directory_entry& item : fsys::directory_iterator(realPath, error))
    {
        bool isDirectory = item.is_directory(error);
        if (isDirectory && !(mode & OpenDirectoryMode_Directory)) continue;
        if (!isDirectory && !(mode & OpenDirectoryMode_File)) continue;

        DirectoryEntry entry = {};
        std::string name = item.path().filename().string();
        std::strncpy(entry.name, name.c_str(), sizeof(entry.name) - 1);
        entry.type = isDirectory ? DirectoryEntryType_Directory : DirectoryEntryType_File;
        entry.fileSize = isDirectory ? 0 : static_cast<int64_t>(item.file_size(error));
        object->entries.push_back(entry);
    }
    outHandle->handle = object;
    return Result{};
}

Result ReadDirectory(int64_t* outCount, DirectoryEntry* entries, DirectoryHandle handle, int64_t maxCount)
{
    detail::DirectoryObject* object = static_cast<detail::DirectoryObject*>(handle.handle);
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
    delete static_cast<detail::DirectoryObject*>(handle.handle);
}

}
