#include "nn/oe.h"
#include <cstring>

namespace nn::oe {

bool TryPopNotificationMessage(uint32_t* outMessage)
{
    //Pas de message de la console sur PC (mise en veille, changement de mode...)
    *outMessage = 0;
    return false;
}

void SetOperationModeChangedNotificationEnabled(bool enabled)
{
    (void)enabled;
}

void SetResumeNotificationEnabled(bool enabled)
{
    (void)enabled;
}

OperationMode GetOperationMode()
{
    //Sur PC, on fait comme si la console etait sur la TV
    return OperationMode_Console;
}

nn::settings::LanguageCode GetDesiredLanguage()
{
    //Le jeu est la version japonaise : il cherche les fichiers en _ja
    nn::settings::LanguageCode code;
    std::strcpy(code.string, "ja");
    return code;
}

void GetPseudoDeviceId(nn::util::Uuid* outId)
{
    //Identifiant de la console : tout a zero sur PC
    *outId = nn::util::Uuid{};
}

void EnableRecording()
{
    //Capture video de la Switch : rien a faire
}

void DisableRecording()
{
}

void SetUserInactivityDetectionTimeExtended(bool extended)
{
    //Mise en veille automatique : rien a faire sur PC
    (void)extended;
}

void SetUserInactivityDetectionTimeExtendedUnsafe(bool extended)
{
    (void)extended;
}

}
