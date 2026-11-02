#include "nn/fs/file.h"
#include "nn/fs/detail.h"
#include "nn/fs/result.h"
#include <cstdio>
#include <filesystem>
#include <mutex>
#include <set>

namespace nn::fs {

//Tous les fichiers ouverts, pour pouvoir les vider dans CommitSaveData
static std::set<detail::FileObject*> g_OpenFiles;
static std::mutex g_OpenFilesLock;

void detail::FlushAllFiles()
{
    std::lock_guard<std::mutex> guard(g_OpenFilesLock);
    for (detail::FileObject* object : g_OpenFiles)
    {
        std::lock_guard<std::mutex> fileGuard(object->lock);
        std::fflush(object->file);
    }
}

//fseek ne va que jusqu'a 2 Go sous Windows, et yw1_a.fa fait 3,4 Go : on prend la version 64 bits
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

Result OpenFile(FileHandle* outHandle, const char* path, int mode)
{
    std::string realPath = detail::ResolvePath(path);
    if (realPath.empty())
    {
        return ResultPathNotFound;
    }

    //Comme sur Switch, OpenFile ne cree pas et ne vide pas le fichier (c'est CreateFile)
    const char* fopenMode = "rb";
    if (mode & OpenMode_Write)
    {
        fopenMode = "r+b";
    }
    FILE* file = std::fopen(realPath.c_str(), fopenMode);
    if (file == nullptr)
    {
        return ResultPathNotFound;
    }

    detail::FileObject* object = new detail::FileObject();
    object->file = file;
    object->path = realPath;
    {
        std::lock_guard<std::mutex> guard(g_OpenFilesLock);
        g_OpenFiles.insert(object);
    }
    outHandle->handle = object;
    return Result{};
}

void CloseFile(FileHandle handle)
{
    detail::FileObject* object = (detail::FileObject*)handle.handle;
    if (object == nullptr)
    {
        return;
    }
    {
        std::lock_guard<std::mutex> guard(g_OpenFilesLock);
        g_OpenFiles.erase(object);
    }
    std::fclose(object->file);
    delete object;
}

Result ReadFile(size_t* outReadSize, FileHandle handle, int64_t offset, void* buffer, size_t size)
{
    detail::FileObject* object = (detail::FileObject*)handle.handle;
    //Le verrou empeche un autre thread de bouger la position entre le Seek et le fread
    std::lock_guard<std::mutex> guard(object->lock);
    size_t readSize = 0;
    if (Seek(object->file, offset, SEEK_SET) == 0)
    {
        readSize = std::fread(buffer, 1, size, object->file);
    }
    if (outReadSize != nullptr)
    {
        *outReadSize = readSize;
    }
    return Result{};
}

Result ReadFile(FileHandle handle, int64_t offset, void* buffer, size_t size)
{
    return ReadFile(nullptr, handle, offset, buffer, size);
}

Result WriteFile(FileHandle handle, int64_t offset, const void* buffer, size_t size, const WriteOption& option)
{
    detail::FileObject* object = (detail::FileObject*)handle.handle;
    std::lock_guard<std::mutex> guard(object->lock);
    if (Seek(object->file, offset, SEEK_SET) != 0)
    {
        return ResultPathNotFound;
    }
    std::fwrite(buffer, 1, size, object->file);
    if (option.flags & WriteOptionFlag_Flush)
    {
        std::fflush(object->file);
    }
    return Result{};
}

Result GetFileSize(int64_t* outSize, FileHandle handle)
{
    detail::FileObject* object = (detail::FileObject*)handle.handle;
    std::lock_guard<std::mutex> guard(object->lock);
    Seek(object->file, 0, SEEK_END);
    *outSize = Tell(object->file);
    return Result{};
}

Result SetFileSize(FileHandle handle, int64_t size)
{
    detail::FileObject* object = (detail::FileObject*)handle.handle;
    std::lock_guard<std::mutex> guard(object->lock);
    std::fflush(object->file);
    std::error_code error;
    std::filesystem::resize_file(object->path, (uintmax_t)size, error);
    if (error)
    {
        return ResultPathNotFound;
    }
    return Result{};
}

Result FlushFile(FileHandle handle)
{
    detail::FileObject* object = (detail::FileObject*)handle.handle;
    std::lock_guard<std::mutex> guard(object->lock);
    std::fflush(object->file);
    return Result{};
}

}
