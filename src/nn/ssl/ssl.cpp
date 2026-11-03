#include "nn/ssl.h"
#include "nn/offline.h"

namespace nn::ssl {

Result Context::Create(SslVersion version)
{
    (void)version;
    return ResultOffline;
}

Result Context::ImportServerPki(uint64_t* outId, const char* certificate, uint32_t size, CertificateFormat format)
{
    (void)certificate;
    (void)size;
    (void)format;
    *outId = 0;
    return ResultOffline;
}

}
