#include "nn/ldn.h"
#include "nn/offline.h"

namespace nn::ldn {

Result Initialize()
{
    return ResultOffline; //pas de communication locale : le jeu desactive ce mode
}

void Finalize()
{
}

State GetState()
{
    return State_None;
}

DisconnectReason GetDisconnectReason()
{
    return DisconnectReason_None;
}

Result AttachStateChangeEvent(nn::os::SystemEventType* outEvent)
{
    nn::os::CreateSystemEvent(outEvent, nn::os::EventClearMode_AutoClear, false);
    return Result{};
}

Result OpenAccessPoint()
{
    return ResultOffline;
}

Result CloseAccessPoint()
{
    return ResultOffline;
}

Result CreateNetwork(const NetworkConfig& network, const SecurityConfig& security, const UserConfig& user)
{
    (void)network;
    (void)security;
    (void)user;
    return ResultOffline;
}

Result CreateNetworkPrivate(const NetworkConfig& network, const SecurityConfig& security, const SecurityParameter& parameter,
                            const UserConfig& user, int addressCount, const AddressEntry* addresses)
{
    (void)network;
    (void)security;
    (void)parameter;
    (void)user;
    (void)addressCount;
    (void)addresses;
    return ResultOffline;
}

Result DestroyNetwork()
{
    return ResultOffline;
}

Result SetAdvertiseData(const void* data, size_t size)
{
    (void)data;
    (void)size;
    return ResultOffline;
}

Result SetStationAcceptPolicy(AcceptPolicy policy)
{
    (void)policy;
    return ResultOffline;
}

Result Reject(Ipv4Address address)
{
    (void)address;
    return ResultOffline;
}

Result OpenStation()
{
    return ResultOffline;
}

Result CloseStation()
{
    return ResultOffline;
}

Result Connect(const NetworkInfo& network, const SecurityConfig& security, const UserConfig& user,
               int version, ConnectOption option)
{
    (void)network;
    (void)security;
    (void)user;
    (void)version;
    (void)option;
    return ResultOffline;
}

Result ConnectPrivate(const NetworkConfig& network, const SecurityConfig& security, const SecurityParameter& parameter,
                      const UserConfig& user, int version, ConnectOption option)
{
    (void)network;
    (void)security;
    (void)parameter;
    (void)user;
    (void)version;
    (void)option;
    return ResultOffline;
}

Result Disconnect()
{
    return ResultOffline;
}

Result GetNetworkInfo(NetworkInfo* outInfo)
{
    (void)outInfo;
    return ResultOffline;
}

Result GetNetworkConfig(NetworkConfig* outConfig)
{
    (void)outConfig;
    return ResultOffline;
}

Result GetSecurityParameter(SecurityParameter* outParameter)
{
    (void)outParameter;
    return ResultOffline;
}

Result GetIpv4Address(Ipv4Address* outAddress, SubnetMask* outMask)
{
    (void)outAddress;
    (void)outMask;
    return ResultOffline;
}

Result Scan(NetworkInfo* outNetworks, int* outCount, int maxCount, const ScanFilter& filter, int channel)
{
    (void)outNetworks;
    (void)maxCount;
    (void)filter;
    (void)channel;
    *outCount = 0; //aucune console trouvee
    return ResultOffline;
}

Result ScanPrivate(NetworkInfo* outNetworks, int* outCount, int maxCount, const ScanFilter& filter, int channel)
{
    return Scan(outNetworks, outCount, maxCount, filter, channel);
}

Ipv4Address MakeIpv4Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    Ipv4Address address;
    address.raw = ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | d;
    return address;
}

Ipv4Address MakeBroadcastAddress(Ipv4Address address, SubnetMask mask)
{
    Ipv4Address broadcast;
    broadcast.raw = address.raw | ~mask.raw;
    return broadcast;
}

}
