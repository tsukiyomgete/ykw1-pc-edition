#include "nn/util.h"
#include <cstdio>

namespace nn::util {

void ReferSymbol(const void* symbol)
{
    (void)symbol;
}

int VSNPrintf(char* buffer, size_t size, const char* format, va_list args)
{
    return std::vsnprintf(buffer, size, format, args);
}

int SNPrintf(char* buffer, size_t size, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = std::vsnprintf(buffer, size, format, args);
    va_end(args);
    return result;
}

int TSNPrintf(char* buffer, size_t size, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = std::vsnprintf(buffer, size, format, args);
    va_end(args);
    return result;
}

}
