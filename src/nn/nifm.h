#pragma once
#include <cstdint>
#include "nn/types.h"
#include "nn/socket.h"

//Connexion a Internet : sur PC, le jeu est hors ligne
namespace nn::nifm
{
    struct ProxySetting {
        bool enabled = false;
        uint16_t port = 0;
        char server[100] = {};
    };

    Result Initialize();
    void   SubmitNetworkRequest();
    void   CancelNetworkRequest();
    bool   IsNetworkRequestOnHold();
    bool   IsNetworkAvailable();
    Result HandleNetworkRequestErrorResult();
    Result GetCurrentPrimaryIpAddress(nn::socket::InAddr* outAddress);
    Result GetCurrentProxySetting(ProxySetting* outSetting);
    Result SetLocalNetworkMode(bool enabled);
}
