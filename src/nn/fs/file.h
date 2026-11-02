#pragma once
#include <cstddef>
#include <cstdint>
#include "nn/types.h"

namespace nn::fs
{
    struct FileHandle {
        void* handle = nullptr;
    };

    enum OpenMode {
        OpenMode_Read        = 1,
        OpenMode_Write       = 2,
        OpenMode_AllowAppend = 4,
    };

    enum WriteOptionFlag {
        WriteOptionFlag_Flush = 1,
    };

    struct WriteOption {
        int flags = 0;
    };

    Result OpenFile(FileHandle* outHandle, const char* path, int mode);
    void   CloseFile(FileHandle handle);
    Result ReadFile(FileHandle handle, int64_t offset, void* buffer, size_t size);
    Result ReadFile(size_t* outReadSize, FileHandle handle, int64_t offset, void* buffer, size_t size);
    Result WriteFile(FileHandle handle, int64_t offset, const void* buffer, size_t size, const WriteOption& option);
    Result GetFileSize(int64_t* outSize, FileHandle handle);
    Result SetFileSize(FileHandle handle, int64_t size);
    Result FlushFile(FileHandle handle);
}
