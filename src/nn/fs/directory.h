#pragma once
#include <cstdint>
#include "nn/types.h"

namespace nn::fs
{
    struct DirectoryHandle {
        void* handle = nullptr;
    };

    enum DirectoryEntryType : int8_t {
        DirectoryEntryType_Directory = 0,
        DirectoryEntryType_File      = 1,
    };

    enum OpenDirectoryMode {
        OpenDirectoryMode_Directory = 1,
        OpenDirectoryMode_File      = 2,
        OpenDirectoryMode_All       = 3,
    };

    struct DirectoryEntry {
        char name[769];
        char reserved1[3];
        DirectoryEntryType type;
        char reserved2[3];
        int64_t fileSize;
    };

    Result CreateFile(const char* path, int64_t size);
    Result DeleteFile(const char* path);
    Result RenameFile(const char* currentPath, const char* newPath);
    Result CreateDirectory(const char* path);
    Result GetEntryType(DirectoryEntryType* outType, const char* path);
    Result OpenDirectory(DirectoryHandle* outHandle, const char* path, int mode);
    Result ReadDirectory(int64_t* outCount, DirectoryEntry* entries, DirectoryHandle handle, int64_t maxCount);
    void   CloseDirectory(DirectoryHandle handle);
}
