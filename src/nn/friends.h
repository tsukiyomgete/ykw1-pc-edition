#pragma once
#include <cstdint>
#include "nn/types.h"
#include "nn/account.h"

//Amis Nintendo : pas disponible sur PC
namespace nn::friends
{
    struct PlayHistoryRegistrationKey {
        uint8_t data[64] = {};
    };

    struct InAppScreenName {
        char name[64] = {};
        char language[8] = {};
    };

    struct FriendFilter {
        int presenceStatus = 0;
        bool isFavoriteOnly = false;
    };

    class Friend {
    public:
        Friend();
        uint64_t GetAccountId() const;
    };

    Result GetFriendList(int* outCount, Friend* outFriends, const nn::account::Uid& user,
                         int offset, int count, const FriendFilter& filter);
    Result GetPlayHistoryRegistrationKey(PlayHistoryRegistrationKey* outKey, const nn::account::Uid& user, bool isLocalPlay);
    Result AddPlayHistory(const nn::account::Uid& user, const PlayHistoryRegistrationKey& key,
                          const InAppScreenName& name, const InAppScreenName& myName);
}
