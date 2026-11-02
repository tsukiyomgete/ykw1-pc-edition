#include "nn/hid/vibration.h"
#include "nn/hid/sdl_input.h"

namespace nn::hid {

int GetVibrationDeviceHandles(VibrationDeviceHandle* outHandles, int count, const NpadIdType& id, NpadStyleSet style)
{
    //2 moteurs comme une Pro Controller : 0 = gauche, 1 = droit
    (void)style;
    int number = 2;
    if (count < 2)
    {
        number = count;
    }
    for (int i = 0; i < number; i++)
    {
        outHandles[i].value = (id << 8) | i;
    }
    return number;
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
    //Une manette PC a un moteur grave et un moteur aigu
    (void)handle;
    detail::Rumble(value.amplitudeLow, value.amplitudeHigh);
}

}
