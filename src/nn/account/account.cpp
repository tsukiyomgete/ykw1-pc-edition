#include "nn/account.h"
#include "nn/offline.h"
#include <cstring>

namespace nn::account {

//L'unique joueur du PC
static Uid MakePlayerUid()
{
    Uid uid;
    uid.value[0] = 1;
    uid.value[1] = 0;
    return uid;
}

AsyncContext::AsyncContext()
{
}

AsyncContext::~AsyncContext()
{
}

Result AsyncContext::Cancel()
{
    return Result{};
}

Result AsyncContext::GetResult()
{
    return ResultOffline;
}

Result AsyncContext::GetSystemEvent(nn::os::SystemEvent* outEvent)
{
    //L'operation est "deja finie" : on donne un event deja signale
    nn::os::CreateSystemEvent(&outEvent->base, nn::os::EventClearMode_ManualClear, false);
    nn::os::SignalSystemEvent(&outEvent->base);
    return Result{};
}

Result AsyncContext::HasDone(bool* outDone)
{
    *outDone = true;
    return Result{};
}

Result Initialize()
{
    return Result{};
}

Result GetUserCount(int* outCount)
{
    *outCount = 1;
    return Result{};
}

Result GetUserExistence(bool* outExists, const Uid& user)
{
    *outExists = (user.value[0] == 1 && user.value[1] == 0);
    return Result{};
}

Result OpenPreselectedUser(UserHandle* outHandle)
{
    outHandle->uid = MakePlayerUid();
    return Result{};
}

Result OpenUser(UserHandle* outHandle, const Uid& user)
{
    outHandle->uid = user;
    return Result{};
}

void CloseUser(const UserHandle& handle)
{
    (void)handle;
}

Result GetUserId(Uid* outUser, const UserHandle& handle)
{
    *outUser = handle.uid;
    return Result{};
}

Result GetNickname(Nickname* outName, const Uid& user)
{
    (void)user;
    std::strcpy(outName->name, "Player");
    return Result{};
}

Result EnsureNetworkServiceAccountAvailable(const UserHandle& handle)
{
    (void)handle;
    return ResultOffline;
}

Result EnsureNetworkServiceAccountIdTokenCacheAsync(AsyncContext* outContext, const UserHandle& handle)
{
    (void)outContext;
    (void)handle;
    return ResultOffline;
}

Result GetNetworkServiceAccountId(NetworkServiceAccountId* outId, const UserHandle& handle)
{
    (void)handle;
    outId->id = 0;
    return ResultOffline;
}

Result LoadNetworkServiceAccountIdTokenCache(size_t* outSize, char* buffer, size_t bufferSize, const UserHandle& handle)
{
    (void)buffer;
    (void)bufferSize;
    (void)handle;
    *outSize = 0;
    return ResultOffline;
}

}
