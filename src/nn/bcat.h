#pragma once
#include <cstddef>
#include <cstdint>
#include "nn/types.h"

//BCAT : contenus telecharges en arriere-plan depuis les serveurs de Nintendo. Rien sur PC.
namespace nn::bcat
{
    struct DirectoryName {
        char value[32] = {};
    };

    enum DeliveryCacheProgressStatus {
        DeliveryCacheProgressStatus_None = 0,
        DeliveryCacheProgressStatus_Done = 9,
    };

    class DeliveryCacheProgress {
    public:
        DeliveryCacheProgress();
        void Update();
        DeliveryCacheProgressStatus GetStatus() const;
        Result GetResult() const;
    };

    class DeliveryCacheFile {
    public:
        DeliveryCacheFile();
        ~DeliveryCacheFile();
        Result Open(const char* path);
        void Close();
        int64_t GetSize();
        Result Read(size_t* outSize, int64_t offset, void* buffer, size_t size);
    };

    Result Initialize();
    Result MountDeliveryCacheStorage();
    void   UnmountDeliveryCacheStorage();
    Result RequestSyncDeliveryCache(DeliveryCacheProgress* outProgress);
    Result EnumerateDeliveryCacheDirectory(int* outCount, DirectoryName* outNames, int maxCount);
}
