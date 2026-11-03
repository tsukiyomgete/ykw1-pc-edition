#pragma once
#include "nn/types.h"

namespace nn
{
    //Erreur renvoyee par tout ce qui ne marche pas sur PC (Nintendo Online, NFC, contenu additionnel...)
    inline constexpr Result ResultOffline = { 0x1 };
}
