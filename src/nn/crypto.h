#pragma once
#include <cstddef>
#include <cstdint>

namespace nn::crypto
{
    void GenerateCryptographicallyRandomBytes(void* buffer, size_t size);

    //Empreintes : SHA-1 (20 octets) et HMAC-SHA256 (32 octets)
    void GenerateSha1Hash(void* outHash, size_t hashSize, const void* data, size_t dataSize);
    void GenerateHmacSha256Mac(void* outMac, size_t macSize, const void* data, size_t dataSize,
                               const void* key, size_t keySize);

    //Chiffrement AES-128 en mode CTR (chiffrer et dechiffrer font la meme operation)
    size_t EncryptAes128Ctr(void* dst, size_t dstSize, const void* key, size_t keySize,
                            const void* iv, size_t ivSize, const void* src, size_t srcSize);
    size_t DecryptAes128Ctr(void* dst, size_t dstSize, const void* key, size_t keySize,
                            const void* iv, size_t ivSize, const void* src, size_t srcSize);

    namespace detail
    {
        //Empreinte MD5 (16 octets), calculee petit a petit
        class Md5Impl {
        public:
            ~Md5Impl();
            void Initialize();
            void Update(const void* data, size_t size);
            void GetHash(void* outHash, size_t hashSize);

        private:
            uint32_t state[4];
            uint64_t totalSize;
            uint8_t block[64];
            size_t blockUsed;
        };
    }
}
