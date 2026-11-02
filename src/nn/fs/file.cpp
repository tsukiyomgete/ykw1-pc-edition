#include "nn/fs/file.h"
#include "nn/fs/detail.h"
#include "nn/fs/result.h"
#include <cstdio>
#include <filesystem>
#include <mutex>
#include <set>

namespace nn::fs {

namespace detail {

// Every open file, so CommitSaveData can flush them
static std::set<FileObject*> g_OpenFiles;
static std::mutex g_OpenFilesLock;

void FlushAllFiles()
{
    std::lock_guard<std::mutex> guard(g_OpenFilesLock);
    for (FileObject* object : g_OpenFiles)
    {
        std::lock_guard<std::mutex> fileGuard(object->lock);
        std::fflush(object->file);
    }
}

// fseek/ftell use a 32-bit long on Windows: yw1_a.fa is 3.4 GB, so use the 64-bit versions
static int Seek(FILE* file, int64_t offset, int origin)
{
#ifdef _WIN32
    return _fseeki64(file, offset, origin);
#else
    return fseeko(file, offset, origin);
#endif
}

static int64_t Tell(FILE* file)
{
#ifdef _WIN32
    return _ftelli64(file);
#else
    return ftello(file);
#endif
}

static FileObject* Get(FileHandle handle)
{
    return static_cast<FileObject*>(handle.handle);
}

}

Result OpenFile(FileHandle* outHandle, const char* path, int mode)
{
    std::string realPath = detail::ResolvePath(path);
    if (realPath.empty())
        return ResultPathNotFound;

    // Like on Switch, OpenFile never creates nor empties a file (that is CreateFile's job)
    const char* fopenMode = (mode & OpenMode_Write) ? "r+b" : "rb";
    FILE* file = std::fopen(realPath.c_str(), fopenMode);
    if (file == nullptr)
        return ResultPathNotFound;

    detail::FileObject* object = new detail::FileObject();
    object->file = file;
    object->path = realPath;
    {
        std::lock_guard<std::mutex> guard(detail::g_OpenFilesLock);
        detail::g_OpenFiles.insert(object);
    }
    outHandle->handle = object;
    return Result{};
}

void CloseFile(FileHandle handle)
{
    detail::FileObject* object = detail::Get(handle);
    if (object == nullptr)
        return;
    {
        std::lock_guard<std::mutex> guard(detail::g_OpenFilesLock);
        detail::g_OpenFiles.erase(object);
    }
    std::fclose(object->file);
    delete object;
}

Result ReadFile(size_t* outReadSize, FileHandle handle, int64_t offset, void* buffer, size_t size)
{
    detail::FileObject* object = detail::Get(handle);
    std::lock_guard<std::mutex> guard(object->lock);
    size_t read = 0;
    if (detail::Seek(object->file, offset, SEEK_SET) == 0)
        read = std::fread(buffer, 1, size, object->file);
    if (outReadSize != nullptr)
        *outReadSize = read;
    return Result{};
}

Result ReadFile(FileHandle handle, int64_t offset, void* buffer, size_t size)
{
    return ReadFile(nullptr, handle, offset, buffer, size);
}

Result WriteFile(FileHandle handle, int64_t offset, const void* buffer, size_t size, const WriteOption& option)
{
    detail::FileObject* object = detail::Get(handle);
    std::lock_guard<std::mutex> guard(object->lock);
    if (detail::Seek(object->file, offset, SEEK_SET) != 0)
        return ResultPathNotFound;
    std::fwrite(buffer, 1, size, object->file);
    if (option.flags & WriteOptionFlag_Flush)
        std::fflush(object->file);
    return Result{};
}

Result GetFileSize(int64_t* outSize, FileHandle handle)
{
    detail::FileObject* object = detail::Get(handle);
    std::lock_guard<std::mutex> guard(object->lock);
    detail::Seek(object->file, 0, SEEK_END);
    *outSize = detail::Tell(object->file);
    return Result{};
}

Result SetFileSize(FileHandle handle, int64_t size)
{
    detail::FileObject* object = detail::Get(handle);
    std::lock_guard<std::mutex> guard(object->lock);
    std::fflush(object->file);
    std::error_code error;
    std::filesystem::resize_file(object->path, static_cast<uintmax_t>(size), error);
    return error ? ResultPathNotFound : Result{};
}

Result FlushFile(FileHandle handle)
{
    detail::FileObject* object = detail::Get(handle);
    std::lock_guard<std::mutex> guard(object->lock);
    std::fflush(object->file);
    return Result{};
}

}
