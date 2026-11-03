#include "nn/diag.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace nn::diag::detail {

void AbortImpl(const char* condition, const char* function, const char* file, int line)
{
    std::fprintf(stderr, "ABORT: %s\n  dans %s (%s:%d)\n", condition, function, file, line);
    std::abort();
}

void AbortImpl(const char* condition, const char* function, const char* file, int line,
               const Result* result, const char* format, ...)
{
    std::fprintf(stderr, "ABORT: %s\n  dans %s (%s:%d)\n", condition, function, file, line);
    if (result != nullptr)
    {
        std::fprintf(stderr, "  Result : 0x%x\n", result->value);
    }
    if (format != nullptr)
    {
        va_list args;
        va_start(args, format);
        std::vfprintf(stderr, format, args);
        va_end(args);
        std::fprintf(stderr, "\n");
    }
    std::abort();
}

}
