#include "nn/bcat.h"
#include "nn/offline.h"

namespace nn::bcat {

DeliveryCacheProgress::DeliveryCacheProgress()
{
}

void DeliveryCacheProgress::Update()
{
}

DeliveryCacheProgressStatus DeliveryCacheProgress::GetStatus() const
{
    return DeliveryCacheProgressStatus_Done; //"fini" tout de suite, pour que le jeu n'attende pas
}

Result DeliveryCacheProgress::GetResult() const
{
    return ResultOffline;
}

DeliveryCacheFile::DeliveryCacheFile()
{
}

DeliveryCacheFile::~DeliveryCacheFile()
{
}

Result DeliveryCacheFile::Open(const char* path)
{
    (void)path;
    return ResultOffline;
}

void DeliveryCacheFile::Close()
{
}

int64_t DeliveryCacheFile::GetSize()
{
    return 0;
}

Result DeliveryCacheFile::Read(size_t* outSize, int64_t offset, void* buffer, size_t size)
{
    (void)offset;
    (void)buffer;
    (void)size;
    *outSize = 0;
    return ResultOffline;
}

Result Initialize()
{
    return Result{};
}

Result MountDeliveryCacheStorage()
{
    return ResultOffline;
}

void UnmountDeliveryCacheStorage()
{
}

Result RequestSyncDeliveryCache(DeliveryCacheProgress* outProgress)
{
    (void)outProgress;
    return ResultOffline;
}

Result EnumerateDeliveryCacheDirectory(int* outCount, DirectoryName* outNames, int maxCount)
{
    (void)outNames;
    (void)maxCount;
    *outCount = 0;
    return ResultOffline;
}

}
