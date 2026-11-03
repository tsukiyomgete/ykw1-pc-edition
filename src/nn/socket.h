#pragma once
#include <cstddef>
#include <cstdint>
#include "nn/types.h"

//Sockets (Internet) : sur PC le jeu est hors ligne, toutes les connexions echouent.
//Les types sont les notres (pas ceux de Windows) pour eviter d'inclure winsock.
namespace nn::socket
{
    struct InAddr {
        uint32_t address = 0;
    };

    struct SockAddr {
        uint8_t length = 0;
        uint8_t family = 0;
        char data[14] = {};
    };

    struct AddrInfo {
        int flags = 0;
        int family = 0;
        int socketType = 0;
        int protocol = 0;
        uint32_t addressLength = 0;
        SockAddr* address = nullptr;
        char* canonicalName = nullptr;
        AddrInfo* next = nullptr;
    };

    struct PollFd {
        int fd = 0;
        short events = 0;
        short revents = 0;
    };

    struct Config {
        void* memoryPool = nullptr;
        size_t memoryPoolSize = 0;
    };

    using Family = int;
    using Type = int;
    using Protocol = int;
    using Level = int;
    using Option = int;
    using MsgFlag = int;
    using AiErrno = int;
    using Errno = int;

    Result Initialize(const Config& config);
    Result Finalize();
    Errno  GetLastErrno();

    int Socket(Family family, Type type, Protocol protocol);
    int Close(int socket);
    int Shutdown(int socket, int how);
    int Bind(int socket, const SockAddr* address, uint32_t addressLength);
    int Listen(int socket, int backlog);
    int Accept(int socket, SockAddr* outAddress, uint32_t* outAddressLength);
    int Connect(int socket, const SockAddr* address, uint32_t addressLength);
    int Send(int socket, const void* buffer, size_t size, int flags);
    int SendTo(int socket, const void* buffer, size_t size, MsgFlag flags, const SockAddr* address, uint32_t addressLength);
    int Recv(int socket, void* buffer, size_t size, int flags);
    int RecvFrom(int socket, void* buffer, size_t size, MsgFlag flags, SockAddr* outAddress, uint32_t* outAddressLength);
    int Poll(PollFd* fds, size_t count, int timeout);
    int Fcntl(int socket, int command, ...);
    int GetSockName(int socket, SockAddr* outAddress, uint32_t* outAddressLength);
    int GetSockOpt(int socket, Level level, Option option, void* outValue, uint32_t* outValueLength);
    int SetSockOpt(int socket, Level level, Option option, const void* value, uint32_t valueLength);

    AiErrno GetAddrInfo(const char* node, const char* service, const AddrInfo* hints, AddrInfo** outResult);
    void    FreeAddrInfo(AddrInfo* info);
    const char* GAIStrError(AiErrno error);

    //Conversions d'adresses et d'octets : elles marchent vraiment
    uint32_t InetHtonl(uint32_t value);
    uint16_t InetHtons(uint16_t value);
    uint32_t InetNtohl(uint32_t value);
    uint16_t InetNtohs(uint16_t value);
    int InetAton(const char* text, InAddr* outAddress);
    char* InetNtoa(InAddr address);
    int InetPton(int family, const char* text, void* outAddress);
    const char* InetNtop(int family, const void* address, char* outText, uint32_t textSize);
}
