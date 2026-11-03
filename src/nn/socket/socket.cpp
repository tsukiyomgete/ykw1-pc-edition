#include "nn/socket.h"
#include "nn/offline.h"
#include <cstdio>

namespace nn::socket {

const int ERRNO_NETWORK_DOWN = 50; //ENETDOWN : "le reseau n'est pas disponible"
const int EAI_FAIL = 4;

Result Initialize(const Config& config)
{
    (void)config;
    return Result{};
}

Result Finalize()
{
    return Result{};
}

Errno GetLastErrno()
{
    return ERRNO_NETWORK_DOWN;
}

//Toutes les operations reseau echouent avec -1, comme une vraie socket sans connexion
int Socket(Family family, Type type, Protocol protocol)
{
    (void)family;
    (void)type;
    (void)protocol;
    return -1;
}

int Close(int socket)
{
    (void)socket;
    return -1;
}

int Shutdown(int socket, int how)
{
    (void)socket;
    (void)how;
    return -1;
}

int Bind(int socket, const SockAddr* address, uint32_t addressLength)
{
    (void)socket;
    (void)address;
    (void)addressLength;
    return -1;
}

int Listen(int socket, int backlog)
{
    (void)socket;
    (void)backlog;
    return -1;
}

int Accept(int socket, SockAddr* outAddress, uint32_t* outAddressLength)
{
    (void)socket;
    (void)outAddress;
    (void)outAddressLength;
    return -1;
}

int Connect(int socket, const SockAddr* address, uint32_t addressLength)
{
    (void)socket;
    (void)address;
    (void)addressLength;
    return -1;
}

int Send(int socket, const void* buffer, size_t size, int flags)
{
    (void)socket;
    (void)buffer;
    (void)size;
    (void)flags;
    return -1;
}

int SendTo(int socket, const void* buffer, size_t size, MsgFlag flags, const SockAddr* address, uint32_t addressLength)
{
    (void)socket;
    (void)buffer;
    (void)size;
    (void)flags;
    (void)address;
    (void)addressLength;
    return -1;
}

int Recv(int socket, void* buffer, size_t size, int flags)
{
    (void)socket;
    (void)buffer;
    (void)size;
    (void)flags;
    return -1;
}

int RecvFrom(int socket, void* buffer, size_t size, MsgFlag flags, SockAddr* outAddress, uint32_t* outAddressLength)
{
    (void)socket;
    (void)buffer;
    (void)size;
    (void)flags;
    (void)outAddress;
    (void)outAddressLength;
    return -1;
}

int Poll(PollFd* fds, size_t count, int timeout)
{
    (void)fds;
    (void)count;
    (void)timeout;
    return -1;
}

int Fcntl(int socket, int command, ...)
{
    (void)socket;
    (void)command;
    return -1;
}

int GetSockName(int socket, SockAddr* outAddress, uint32_t* outAddressLength)
{
    (void)socket;
    (void)outAddress;
    (void)outAddressLength;
    return -1;
}

int GetSockOpt(int socket, Level level, Option option, void* outValue, uint32_t* outValueLength)
{
    (void)socket;
    (void)level;
    (void)option;
    (void)outValue;
    (void)outValueLength;
    return -1;
}

int SetSockOpt(int socket, Level level, Option option, const void* value, uint32_t valueLength)
{
    (void)socket;
    (void)level;
    (void)option;
    (void)value;
    (void)valueLength;
    return -1;
}

AiErrno GetAddrInfo(const char* node, const char* service, const AddrInfo* hints, AddrInfo** outResult)
{
    (void)node;
    (void)service;
    (void)hints;
    *outResult = nullptr;
    return EAI_FAIL;
}

void FreeAddrInfo(AddrInfo* info)
{
    (void)info;
}

const char* GAIStrError(AiErrno error)
{
    (void)error;
    return "Network not available on PC";
}

//Les reseaux utilisent le gros-boutiste, le PC le petit-boutiste : on inverse les octets
uint32_t InetHtonl(uint32_t value)
{
    return ((value & 0xFF) << 24) | ((value & 0xFF00) << 8) | ((value >> 8) & 0xFF00) | (value >> 24);
}

uint16_t InetHtons(uint16_t value)
{
    return (uint16_t)((value << 8) | (value >> 8));
}

uint32_t InetNtohl(uint32_t value)
{
    return InetHtonl(value);
}

uint16_t InetNtohs(uint16_t value)
{
    return InetHtons(value);
}

int InetAton(const char* text, InAddr* outAddress)
{
    unsigned int a, b, c, d;
    if (std::sscanf(text, "%u.%u.%u.%u", &a, &b, &c, &d) != 4 || a > 255 || b > 255 || c > 255 || d > 255)
    {
        return 0;
    }
    //Ordre du reseau : le premier nombre dans le premier octet
    outAddress->address = a | (b << 8) | (c << 16) | (d << 24);
    return 1;
}

char* InetNtoa(InAddr address)
{
    static char text[16];
    std::snprintf(text, sizeof(text), "%u.%u.%u.%u", address.address & 0xFF, (address.address >> 8) & 0xFF,
                  (address.address >> 16) & 0xFF, address.address >> 24);
    return text;
}

int InetPton(int family, const char* text, void* outAddress)
{
    if (family != 2) //seulement IPv4 (AF_INET = 2)
    {
        return -1;
    }
    return InetAton(text, (InAddr*)outAddress);
}

const char* InetNtop(int family, const void* address, char* outText, uint32_t textSize)
{
    if (family != 2)
    {
        return nullptr;
    }
    const InAddr* in = (const InAddr*)address;
    std::snprintf(outText, textSize, "%s", InetNtoa(*in));
    return outText;
}

}
