#pragma once
#include <cstddef>
#include <cstdint>
#include "nn/types.h"
#include "nn/os/event.h"
#include "nn/account/uid.h"

//Comptes Switch : sur PC il n'y a qu'un seul joueur, et pas de compte Nintendo en ligne
namespace nn::account
{
    struct UserHandle {
        Uid uid;
    };

    struct Nickname {
        char name[33] = {};
    };

    struct NetworkServiceAccountId {
        uint64_t id = 0;
    };

    //Operation en ligne lancee en arriere-plan
    class AsyncContext {
    public:
        AsyncContext();
        ~AsyncContext();
        Result Cancel();
        Result GetResult();
        Result GetSystemEvent(nn::os::SystemEvent* outEvent);
        Result HasDone(bool* outDone);
    };

    Result Initialize();
    Result GetUserCount(int* outCount);
    Result GetUserExistence(bool* outExists, const Uid& user);
    Result OpenPreselectedUser(UserHandle* outHandle);
    Result OpenUser(UserHandle* outHandle, const Uid& user);
    void   CloseUser(const UserHandle& handle);
    Result GetUserId(Uid* outUser, const UserHandle& handle);
    Result GetNickname(Nickname* outName, const Uid& user);

    //Compte Nintendo en ligne : pas disponible sur PC
    Result EnsureNetworkServiceAccountAvailable(const UserHandle& handle);
    Result EnsureNetworkServiceAccountIdTokenCacheAsync(AsyncContext* outContext, const UserHandle& handle);
    Result GetNetworkServiceAccountId(NetworkServiceAccountId* outId, const UserHandle& handle);
    Result LoadNetworkServiceAccountIdTokenCache(size_t* outSize, char* buffer, size_t bufferSize, const UserHandle& handle);
}
