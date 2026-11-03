#pragma once
#include "nn/types.h"

namespace nn::diag::detail
{
    //Appelees quand le jeu detecte une erreur grave : on affiche et on arrete
    [[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, int line);
    [[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, int line,
                                const Result* result, const char* format, ...);
}
