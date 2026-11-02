#include "nn/fs/directory.h"
#include "nn/fs/detail.h"

namespace nn::fs {

Result CreateFile(const char* path, int64_t size)
{
    // TODO
    (void)path; (void)size;
    return Result{};
}

Result DeleteFile(const char* path)
{
    // TODO
    (void)path;
    return Result{};
}

Result RenameFile(const char* currentPath, const char* newPath)
{
    // TODO
    (void)currentPath; (void)newPath;
    return Result{};
}

Result CreateDirectory(const char* path)
{
    // TODO
    (void)path;
    return Result{};
}

Result GetEntryType(DirectoryEntryType* outType, const char* path)
{
    // TODO
    (void)outType; (void)path;
    return Result{};
}

Result OpenDirectory(DirectoryHandle* outHandle, const char* path, int mode)
{
    // TODO
    (void)outHandle; (void)path; (void)mode;
    return Result{};
}

Result ReadDirectory(int64_t* outCount, DirectoryEntry* entries, DirectoryHandle handle, int64_t maxCount)
{
    // TODO
    (void)outCount; (void)entries; (void)handle; (void)maxCount;
    return Result{};
}

void CloseDirectory(DirectoryHandle handle)
{
    // TODO
    (void)handle;
}

}
