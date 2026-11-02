#include "nn/fs/file.h"
#include "nn/fs/detail.h"

namespace nn::fs {

Result OpenFile(FileHandle* outHandle, const char* path, int mode)
{
    // TODO
    (void)outHandle; (void)path; (void)mode;
    return Result{};
}

void CloseFile(FileHandle handle)
{
    // TODO
    (void)handle;
}

Result ReadFile(FileHandle handle, int64_t offset, void* buffer, size_t size)
{
    // TODO
    (void)handle; (void)offset; (void)buffer; (void)size;
    return Result{};
}

Result ReadFile(size_t* outReadSize, FileHandle handle, int64_t offset, void* buffer, size_t size)
{
    // TODO
    (void)outReadSize; (void)handle; (void)offset; (void)buffer; (void)size;
    return Result{};
}

Result WriteFile(FileHandle handle, int64_t offset, const void* buffer, size_t size, const WriteOption& option)
{
    // TODO
    (void)handle; (void)offset; (void)buffer; (void)size; (void)option;
    return Result{};
}

Result GetFileSize(int64_t* outSize, FileHandle handle)
{
    // TODO
    (void)outSize; (void)handle;
    return Result{};
}

Result SetFileSize(FileHandle handle, int64_t size)
{
    // TODO
    (void)handle; (void)size;
    return Result{};
}

Result FlushFile(FileHandle handle)
{
    // TODO
    (void)handle;
    return Result{};
}

}
