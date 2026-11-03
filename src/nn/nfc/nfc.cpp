#include "nn/nfc.h"
#include "nn/offline.h"

namespace nn::nfc {

Result Initialize()
{
    return Result{};
}

void Finalize()
{
}

Result ListDevices(DeviceHandle* outDevices, int* outCount, int maxCount)
{
    (void)outDevices;
    (void)maxCount;
    *outCount = 0; //aucun lecteur NFC
    return ResultOffline;
}

Result GetNpadId(uint32_t* outNpadId, const DeviceHandle& device)
{
    (void)device;
    *outNpadId = 0;
    return ResultOffline;
}

//Les events sont crees mais jamais signales : le jeu attendra une medaille qui ne vient pas
Result AttachAvailabilityChangeEvent(nn::os::SystemEventType* outEvent)
{
    nn::os::CreateSystemEvent(outEvent, nn::os::EventClearMode_AutoClear, false);
    return Result{};
}

Result AttachActivateEvent(nn::os::SystemEventType* outEvent, const DeviceHandle& device)
{
    (void)device;
    nn::os::CreateSystemEvent(outEvent, nn::os::EventClearMode_AutoClear, false);
    return Result{};
}

Result AttachDeactivateEvent(nn::os::SystemEventType* outEvent, const DeviceHandle& device)
{
    (void)device;
    nn::os::CreateSystemEvent(outEvent, nn::os::EventClearMode_AutoClear, false);
    return Result{};
}

Result StartDetection(const DeviceHandle& device)
{
    (void)device;
    return ResultOffline;
}

Result StopDetection(const DeviceHandle& device)
{
    (void)device;
    return Result{};
}

Result GetTagInfo(TagInfo* outInfo, const DeviceHandle& device)
{
    (void)outInfo;
    (void)device;
    return ResultOffline;
}

Result KeepPassThroughSession(const DeviceHandle& device)
{
    (void)device;
    return ResultOffline;
}

Result ReleasePassThroughSession(const DeviceHandle& device)
{
    (void)device;
    return Result{};
}

Result SendCommandByPassThrough(void* outResponse, size_t* outResponseSize, const DeviceHandle& device,
                                const void* command, size_t commandSize, size_t bufferSize, TimeSpan timeout)
{
    (void)outResponse;
    (void)device;
    (void)command;
    (void)commandSize;
    (void)bufferSize;
    (void)timeout;
    *outResponseSize = 0;
    return ResultOffline;
}

}
