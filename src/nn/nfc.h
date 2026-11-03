#pragma once
#include <cstddef>
#include <cstdint>
#include "nn/types.h"
#include "nn/os/event.h"

//NFC (medailles Yo-kai, amiibo) : pas de lecteur sur PC
namespace nn::nfc
{
    struct DeviceHandle {
        uint32_t value = 0;
    };

    struct TagInfo {
        uint8_t uid[10] = {};
        uint8_t uidLength = 0;
        uint32_t protocol = 0;
        uint32_t type = 0;
    };

    Result Initialize();
    void   Finalize();
    Result ListDevices(DeviceHandle* outDevices, int* outCount, int maxCount);
    Result GetNpadId(uint32_t* outNpadId, const DeviceHandle& device);
    Result AttachAvailabilityChangeEvent(nn::os::SystemEventType* outEvent);
    Result AttachActivateEvent(nn::os::SystemEventType* outEvent, const DeviceHandle& device);
    Result AttachDeactivateEvent(nn::os::SystemEventType* outEvent, const DeviceHandle& device);
    Result StartDetection(const DeviceHandle& device);
    Result StopDetection(const DeviceHandle& device);
    Result GetTagInfo(TagInfo* outInfo, const DeviceHandle& device);
    Result KeepPassThroughSession(const DeviceHandle& device);
    Result ReleasePassThroughSession(const DeviceHandle& device);
    Result SendCommandByPassThrough(void* outResponse, size_t* outResponseSize, const DeviceHandle& device,
                                    const void* command, size_t commandSize, size_t bufferSize, TimeSpan timeout);
}
