#include "nn/crypto.h"
#include <cstring>
#include <random>

namespace nn::crypto {

void GenerateCryptographicallyRandomBytes(void* buffer, size_t size)
{
    std::random_device rng;
    uint8_t* bytes = (uint8_t*)buffer;
    for (size_t i = 0; i < size; i++)
    {
        bytes[i] = (uint8_t)(rng() % 256);
    }
}

//Rotation des bits vers la gauche (utilisee par SHA-1 et MD5)
static uint32_t RotateLeft(uint32_t value, int count)
{
    return (value << count) | (value >> (32 - count));
}

static uint32_t RotateRight(uint32_t value, int count)
{
    return (value >> count) | (value << (32 - count));
}

//Ajoute le "padding" commun a SHA-1 et SHA-256 : 0x80, des zeros, puis la taille en bits (gros-boutiste)
static size_t PadMessage(const uint8_t* data, size_t size, uint8_t* out)
{
    std::memcpy(out, data, size);
    size_t total = size + 1;
    out[size] = 0x80;
    while (total % 64 != 56)
    {
        out[total] = 0;
        total++;
    }
    uint64_t bits = (uint64_t)size * 8;
    for (int i = 7; i >= 0; i--)
    {
        out[total] = (uint8_t)(bits >> (i * 8));
        total++;
    }
    return total;
}

// ======================== SHA-1 ========================
void GenerateSha1Hash(void* outHash, size_t hashSize, const void* data, size_t dataSize)
{
    uint8_t* message = new uint8_t[dataSize + 72];
    size_t total = PadMessage((const uint8_t*)data, dataSize, message);

    uint32_t h[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };
    for (size_t chunk = 0; chunk < total; chunk += 64)
    {
        uint32_t w[80];
        for (int i = 0; i < 16; i++)
        {
            const uint8_t* p = message + chunk + i * 4;
            w[i] = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | p[3];
        }
        for (int i = 16; i < 80; i++)
        {
            w[i] = RotateLeft(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
        }
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
        for (int i = 0; i < 80; i++)
        {
            uint32_t f;
            uint32_t k;
            if (i < 20)
            {
                f = (b & c) | (~b & d);
                k = 0x5A827999;
            }
            else if (i < 40)
            {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (i < 60)
            {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else
            {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            uint32_t temp = RotateLeft(a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = RotateLeft(b, 30);
            b = a;
            a = temp;
        }
        h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e;
    }
    delete[] message;

    uint8_t result[20];
    for (int i = 0; i < 5; i++)
    {
        result[i * 4 + 0] = (uint8_t)(h[i] >> 24);
        result[i * 4 + 1] = (uint8_t)(h[i] >> 16);
        result[i * 4 + 2] = (uint8_t)(h[i] >> 8);
        result[i * 4 + 3] = (uint8_t)h[i];
    }
    if (hashSize > 20)
    {
        hashSize = 20;
    }
    std::memcpy(outHash, result, hashSize);
}

// ======================== SHA-256 ========================
static const uint32_t SHA256_K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

static void Sha256(const uint8_t* data, size_t size, uint8_t* out)
{
    uint8_t* message = new uint8_t[size + 72];
    size_t total = PadMessage(data, size, message);

    uint32_t h[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
    for (size_t chunk = 0; chunk < total; chunk += 64)
    {
        uint32_t w[64];
        for (int i = 0; i < 16; i++)
        {
            const uint8_t* p = message + chunk + i * 4;
            w[i] = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | p[3];
        }
        for (int i = 16; i < 64; i++)
        {
            uint32_t s0 = RotateRight(w[i - 15], 7) ^ RotateRight(w[i - 15], 18) ^ (w[i - 15] >> 3);
            uint32_t s1 = RotateRight(w[i - 2], 17) ^ RotateRight(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
        for (int i = 0; i < 64; i++)
        {
            uint32_t S1 = RotateRight(e, 6) ^ RotateRight(e, 11) ^ RotateRight(e, 25);
            uint32_t ch = (e & f) ^ (~e & g);
            uint32_t temp1 = hh + S1 + ch + SHA256_K[i] + w[i];
            uint32_t S0 = RotateRight(a, 2) ^ RotateRight(a, 13) ^ RotateRight(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;
            hh = g; g = f; f = e; e = d + temp1;
            d = c; c = b; b = a; a = temp1 + temp2;
        }
        h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }
    delete[] message;

    for (int i = 0; i < 8; i++)
    {
        out[i * 4 + 0] = (uint8_t)(h[i] >> 24);
        out[i * 4 + 1] = (uint8_t)(h[i] >> 16);
        out[i * 4 + 2] = (uint8_t)(h[i] >> 8);
        out[i * 4 + 3] = (uint8_t)h[i];
    }
}

void GenerateHmacSha256Mac(void* outMac, size_t macSize, const void* data, size_t dataSize,
                           const void* key, size_t keySize)
{
    //HMAC = SHA256((cle ^ opad) + SHA256((cle ^ ipad) + message))
    uint8_t keyBlock[64] = {};
    if (keySize > 64)
    {
        Sha256((const uint8_t*)key, keySize, keyBlock);
    }
    else
    {
        std::memcpy(keyBlock, key, keySize);
    }

    uint8_t* inner = new uint8_t[64 + dataSize];
    for (int i = 0; i < 64; i++)
    {
        inner[i] = keyBlock[i] ^ 0x36;
    }
    std::memcpy(inner + 64, data, dataSize);
    uint8_t innerHash[32];
    Sha256(inner, 64 + dataSize, innerHash);
    delete[] inner;

    uint8_t outer[64 + 32];
    for (int i = 0; i < 64; i++)
    {
        outer[i] = keyBlock[i] ^ 0x5c;
    }
    std::memcpy(outer + 64, innerHash, 32);
    uint8_t result[32];
    Sha256(outer, 64 + 32, result);

    if (macSize > 32)
    {
        macSize = 32;
    }
    std::memcpy(outMac, result, macSize);
}

// ======================== AES-128 ========================
static const uint8_t SBOX[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16,
};

//Multiplication par 2 dans le corps de Galois utilise par AES
static uint8_t Times2(uint8_t x)
{
    if (x & 0x80)
    {
        return (uint8_t)((x << 1) ^ 0x1b);
    }
    return (uint8_t)(x << 1);
}

//Prepare les 11 cles de tour a partir de la cle de 16 octets
static void ExpandKey(const uint8_t* key, uint8_t* roundKeys)
{
    std::memcpy(roundKeys, key, 16);
    uint8_t rcon = 1;
    for (int i = 16; i < 176; i += 4)
    {
        uint8_t t[4] = { roundKeys[i - 4], roundKeys[i - 3], roundKeys[i - 2], roundKeys[i - 1] };
        if (i % 16 == 0)
        {
            uint8_t first = t[0];
            t[0] = (uint8_t)(SBOX[t[1]] ^ rcon);
            t[1] = SBOX[t[2]];
            t[2] = SBOX[t[3]];
            t[3] = SBOX[first];
            rcon = Times2(rcon);
        }
        for (int j = 0; j < 4; j++)
        {
            roundKeys[i + j] = roundKeys[i + j - 16] ^ t[j];
        }
    }
}

//Chiffre un bloc de 16 octets
static void EncryptBlock(const uint8_t* roundKeys, const uint8_t* in, uint8_t* out)
{
    uint8_t s[16];
    for (int i = 0; i < 16; i++)
    {
        s[i] = in[i] ^ roundKeys[i];
    }
    for (int round = 1; round <= 10; round++)
    {
        //SubBytes + ShiftRows
        uint8_t t[16];
        for (int c = 0; c < 4; c++)
        {
            for (int r = 0; r < 4; r++)
            {
                t[c * 4 + r] = SBOX[s[((c + r) % 4) * 4 + r]];
            }
        }
        //MixColumns (sauf au dernier tour)
        if (round != 10)
        {
            for (int c = 0; c < 4; c++)
            {
                uint8_t a0 = t[c * 4], a1 = t[c * 4 + 1], a2 = t[c * 4 + 2], a3 = t[c * 4 + 3];
                uint8_t all = a0 ^ a1 ^ a2 ^ a3;
                t[c * 4 + 0] = a0 ^ all ^ Times2(a0 ^ a1);
                t[c * 4 + 1] = a1 ^ all ^ Times2(a1 ^ a2);
                t[c * 4 + 2] = a2 ^ all ^ Times2(a2 ^ a3);
                t[c * 4 + 3] = a3 ^ all ^ Times2(a3 ^ a0);
            }
        }
        //AddRoundKey
        for (int i = 0; i < 16; i++)
        {
            s[i] = t[i] ^ roundKeys[round * 16 + i];
        }
    }
    std::memcpy(out, s, 16);
}

size_t EncryptAes128Ctr(void* dst, size_t dstSize, const void* key, size_t keySize,
                        const void* iv, size_t ivSize, const void* src, size_t srcSize)
{
    if (keySize != 16 || ivSize != 16)
    {
        return 0;
    }
    size_t size = srcSize;
    if (dstSize < size)
    {
        size = dstSize;
    }
    uint8_t roundKeys[176];
    ExpandKey((const uint8_t*)key, roundKeys);

    //Mode CTR : on chiffre un compteur, puis on fait un XOR avec les donnees
    uint8_t counter[16];
    std::memcpy(counter, iv, 16);
    const uint8_t* in = (const uint8_t*)src;
    uint8_t* out = (uint8_t*)dst;
    for (size_t offset = 0; offset < size; offset += 16)
    {
        uint8_t keyStream[16];
        EncryptBlock(roundKeys, counter, keyStream);
        for (size_t i = 0; i < 16 && offset + i < size; i++)
        {
            out[offset + i] = in[offset + i] ^ keyStream[i];
        }
        //Compteur + 1 (gros-boutiste)
        for (int i = 15; i >= 0; i--)
        {
            counter[i]++;
            if (counter[i] != 0)
            {
                break;
            }
        }
    }
    return size;
}

size_t DecryptAes128Ctr(void* dst, size_t dstSize, const void* key, size_t keySize,
                        const void* iv, size_t ivSize, const void* src, size_t srcSize)
{
    //En mode CTR, dechiffrer = chiffrer
    return EncryptAes128Ctr(dst, dstSize, key, keySize, iv, ivSize, src, srcSize);
}

// ======================== MD5 ========================
static const uint32_t MD5_K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};
static const int MD5_SHIFT[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21,
};

//Traite un bloc de 64 octets
static void Md5Block(uint32_t* state, const uint8_t* block)
{
    uint32_t m[16];
    for (int i = 0; i < 16; i++)
    {
        m[i] = (uint32_t)block[i * 4] | ((uint32_t)block[i * 4 + 1] << 8)
             | ((uint32_t)block[i * 4 + 2] << 16) | ((uint32_t)block[i * 4 + 3] << 24);
    }
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
    for (int i = 0; i < 64; i++)
    {
        uint32_t f;
        int g;
        if (i < 16)
        {
            f = (b & c) | (~b & d);
            g = i;
        }
        else if (i < 32)
        {
            f = (d & b) | (~d & c);
            g = (5 * i + 1) % 16;
        }
        else if (i < 48)
        {
            f = b ^ c ^ d;
            g = (3 * i + 5) % 16;
        }
        else
        {
            f = c ^ (b | ~d);
            g = (7 * i) % 16;
        }
        uint32_t temp = d;
        d = c;
        c = b;
        b = b + RotateLeft(a + f + MD5_K[i] + m[g], MD5_SHIFT[i]);
        a = temp;
    }
    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
}

detail::Md5Impl::~Md5Impl()
{
}

void detail::Md5Impl::Initialize()
{
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
    totalSize = 0;
    blockUsed = 0;
}

void detail::Md5Impl::Update(const void* data, size_t size)
{
    const uint8_t* bytes = (const uint8_t*)data;
    for (size_t i = 0; i < size; i++)
    {
        block[blockUsed] = bytes[i];
        blockUsed++;
        if (blockUsed == 64)
        {
            Md5Block(state, block);
            blockUsed = 0;
        }
    }
    totalSize = totalSize + size;
}

void detail::Md5Impl::GetHash(void* outHash, size_t hashSize)
{
    //Padding : 0x80, des zeros, puis la taille en bits (petit-boutiste)
    uint64_t bits = totalSize * 8;
    uint8_t one = 0x80;
    Update(&one, 1);
    uint8_t zero = 0;
    while (blockUsed != 56)
    {
        Update(&zero, 1);
    }
    uint8_t sizeBytes[8];
    for (int i = 0; i < 8; i++)
    {
        sizeBytes[i] = (uint8_t)(bits >> (i * 8));
    }
    Update(sizeBytes, 8);

    uint8_t result[16];
    for (int i = 0; i < 4; i++)
    {
        result[i * 4 + 0] = (uint8_t)state[i];
        result[i * 4 + 1] = (uint8_t)(state[i] >> 8);
        result[i * 4 + 2] = (uint8_t)(state[i] >> 16);
        result[i * 4 + 3] = (uint8_t)(state[i] >> 24);
    }
    if (hashSize > 16)
    {
        hashSize = 16;
    }
    std::memcpy(outHash, result, hashSize);
}

}
