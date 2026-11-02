#include "nn/hid/vibration.h"
#include "nn/hid/sdl_input.h"

namespace nn::hid {

int GetVibrationDeviceHandles(VibrationDeviceHandle* outHandles, int count, const NpadIdType& id, NpadStyleSet style)
{
    // Two motors, like a Pro Controller: 0 = left, 1 = right
    (void)style;
    int n = count < 2 ? count : 2;
    for (int i = 0; i < n; i++)
        outHandles[i].value = (id << 8) | static_cast<uint32_t>(i);
    return n;
}

void GetVibrationDeviceInfo(VibrationDeviceInfo* outInfo, const VibrationDeviceHandle& handle)
{
    outInfo->deviceType = 1;
    outInfo->position = handle.value & 1;
}

void InitializeVibrationDevice(const VibrationDeviceHandle& handle)
{
    (void)handle;
}

void SendVibrationValue(const VibrationDeviceHandle& handle, const VibrationValue& value)
{
    // A PC gamepad has one low motor and one high motor
    (void)handle;
    detail::Rumble(value.amplitudeLow, value.amplitudeHigh);
}

}
