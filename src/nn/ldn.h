#pragma once
#include <cstddef>
#include <cstdint>
#include "nn/types.h"
#include "nn/os/event.h"

//Communication locale sans fil entre consoles : pas sur PC
namespace nn::ldn
{
    struct Ipv4Address { uint32_t raw = 0; };
    struct SubnetMask { uint32_t raw = 0; };
    struct NetworkInfo { uint8_t data[0x480] = {}; };
    struct NetworkConfig { uint8_t data[0x20] = {}; };
    struct SecurityConfig { uint8_t data[0x44] = {}; };
    struct SecurityParameter { uint8_t data[0x20] = {}; };
    struct UserConfig { char userName[33] = {}; };
    struct AddressEntry { Ipv4Address address; uint8_t macAddress[6] = {}; };
    struct ScanFilter { uint8_t data[0x60] = {}; };

    enum State {
        State_None = 0,
        State_Initialized,
        State_AccessPoint,
        State_AccessPointCreated,
        State_Station,
        State_StationConnected,
        State_Error,
    };
    enum DisconnectReason { DisconnectReason_None = 0 };
    enum ConnectOption { ConnectOption_None = 0 };
    enum AcceptPolicy { AcceptPolicy_AlwaysAccept = 0 };

    Result Initialize();
    void   Finalize();
    State  GetState();
    DisconnectReason GetDisconnectReason();
    Result AttachStateChangeEvent(nn::os::SystemEventType* outEvent);

    Result OpenAccessPoint();
    Result CloseAccessPoint();
    Result CreateNetwork(const NetworkConfig& network, const SecurityConfig& security, const UserConfig& user);
    Result CreateNetworkPrivate(const NetworkConfig& network, const SecurityConfig& security, const SecurityParameter& parameter,
                                const UserConfig& user, int addressCount, const AddressEntry* addresses);
    Result DestroyNetwork();
    Result SetAdvertiseData(const void* data, size_t size);
    Result SetStationAcceptPolicy(AcceptPolicy policy);
    Result Reject(Ipv4Address address);

    Result OpenStation();
    Result CloseStation();
    Result Scan(NetworkInfo* outNetworks, int* outCount, int maxCount, const ScanFilter& filter, int channel);
    Result ScanPrivate(NetworkInfo* outNetworks, int* outCount, int maxCount, const ScanFilter& filter, int channel);
    Result Connect(const NetworkInfo& network, const SecurityConfig& security, const UserConfig& user,
                   int version, ConnectOption option);
    Result ConnectPrivate(const NetworkConfig& network, const SecurityConfig& security, const SecurityParameter& parameter,
                          const UserConfig& user, int version, ConnectOption option);
    Result Disconnect();

    Result GetNetworkInfo(NetworkInfo* outInfo);
    Result GetNetworkConfig(NetworkConfig* outConfig);
    Result GetSecurityParameter(SecurityParameter* outParameter);
    Result GetIpv4Address(Ipv4Address* outAddress, SubnetMask* outMask);
    Ipv4Address MakeIpv4Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
    Ipv4Address MakeBroadcastAddress(Ipv4Address address, SubnetMask mask);
}
