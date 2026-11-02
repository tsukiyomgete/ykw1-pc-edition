#pragma once
#include <cstdint>
#include "nn/hid/npad.h"

namespace nn::hid
{
    struct VibrationDeviceHandle {
        uint32_t value = 0;
    };

    struct VibrationDeviceInfo {
        uint32_t deviceType = 0;
        uint32_t position = 0;   // 0 = left, 1 = right
    };

    struct VibrationValue {
        float amplitudeLow = 0.0f;
        float frequencyLow = 160.0f;
        float amplitudeHigh = 0.0f;
        float frequencyHigh = 320.0f;
    };

    int  GetVibrationDeviceHandles(VibrationDeviceHandle* outHandles, int count, const NpadIdType& id, NpadStyleSet style);
    void GetVibrationDeviceInfo(VibrationDeviceInfo* outInfo, const VibrationDeviceHandle& handle);
    void InitializeVibrationDevice(const VibrationDeviceHandle& handle);
    void SendVibrationValue(const VibrationDeviceHandle& handle, const VibrationValue& value);
}
