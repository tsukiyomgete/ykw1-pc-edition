#pragma once
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include "nn/util/bit_flag_set.h"

namespace nn::util
{
    struct Uuid {
        uint8_t data[16] = {};
    };

    //Sert juste a garder une fonction dans le programme : rien a faire
    void ReferSymbol(const void* symbol);

    //Comme snprintf du C
    int SNPrintf(char* buffer, size_t size, const char* format, ...);
    int TSNPrintf(char* buffer, size_t size, const char* format, ...);
    int VSNPrintf(char* buffer, size_t size, const char* format, va_list args);
}
