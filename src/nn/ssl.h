#pragma once
#include <cstddef>
#include <cstdint>
#include "nn/types.h"

//Connexions securisees (HTTPS) vers Nintendo : pas utilisees sur PC
namespace nn::ssl
{
    enum CertificateFormat {
        CertificateFormat_Pem = 1,
        CertificateFormat_Der = 2,
    };

    class Context {
    public:
        enum SslVersion {
            SslVersion_Auto = 0x01,
            SslVersion_TlsV12 = 0x10,
        };

        Result Create(SslVersion version);
        Result ImportServerPki(uint64_t* outId, const char* certificate, uint32_t size, CertificateFormat format);
    };
}
