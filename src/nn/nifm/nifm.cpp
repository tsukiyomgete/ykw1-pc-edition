#include "nn/nifm.h"
#include "nn/offline.h"

namespace nn::nifm {

Result Initialize()
{
    return Result{};
}

void SubmitNetworkRequest()
{
}

void CancelNetworkRequest()
{
}

bool IsNetworkRequestOnHold()
{
    return false; //la demande est "finie" tout de suite...
}

bool IsNetworkAvailable()
{
    return false; //...et la reponse est : pas de reseau
}

Result HandleNetworkRequestErrorResult()
{
    return ResultOffline;
}

Result GetCurrentPrimaryIpAddress(nn::socket::InAddr* outAddress)
{
    outAddress->address = 0;
    return ResultOffline;
}

Result GetCurrentProxySetting(ProxySetting* outSetting)
{
    *outSetting = ProxySetting{};
    return Result{};
}

Result SetLocalNetworkMode(bool enabled)
{
    (void)enabled;
    return Result{};
}

}
