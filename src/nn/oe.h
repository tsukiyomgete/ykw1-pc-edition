#pragma once
#include <cstdint>
#include "nn/settings.h"
#include "nn/util.h"

namespace nn::oe
{
    enum OperationMode {
        OperationMode_Handheld = 0, //console en portable
        OperationMode_Console = 1,  //console sur le dock (TV)
    };

    bool TryPopNotificationMessage(uint32_t* outMessage);
    void SetOperationModeChangedNotificationEnabled(bool enabled);
    void SetResumeNotificationEnabled(bool enabled);
    OperationMode GetOperationMode();
    nn::settings::LanguageCode GetDesiredLanguage();
    void GetPseudoDeviceId(nn::util::Uuid* outId);
    void EnableRecording();
    void DisableRecording();
    void SetUserInactivityDetectionTimeExtended(bool extended);
    void SetUserInactivityDetectionTimeExtendedUnsafe(bool extended);
}
