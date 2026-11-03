#include "nn/friends.h"
#include "nn/offline.h"

namespace nn::friends {

Friend::Friend()
{
}

uint64_t Friend::GetAccountId() const
{
    return 0;
}

Result GetFriendList(int* outCount, Friend* outFriends, const nn::account::Uid& user,
                     int offset, int count, const FriendFilter& filter)
{
    (void)outFriends;
    (void)user;
    (void)offset;
    (void)count;
    (void)filter;
    *outCount = 0;
    return ResultOffline;
}

Result GetPlayHistoryRegistrationKey(PlayHistoryRegistrationKey* outKey, const nn::account::Uid& user, bool isLocalPlay)
{
    (void)outKey;
    (void)user;
    (void)isLocalPlay;
    return ResultOffline;
}

Result AddPlayHistory(const nn::account::Uid& user, const PlayHistoryRegistrationKey& key,
                      const InAppScreenName& name, const InAppScreenName& myName)
{
    (void)user;
    (void)key;
    (void)name;
    (void)myName;
    return ResultOffline;
}

}
