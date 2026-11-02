#pragma once
#include <cstdint>

namespace nn {

//Valeur renvoyee par la plupart des fonctions du SDK : 0 = succes
struct Result {
    uint32_t value = 0;
    bool IsSuccess() const { return value == 0; }
};

//Une duree, en nanosecondes
struct TimeSpan {
    int64_t nanoSeconds = 0;
    static TimeSpan FromMilliSeconds(int64_t ms) { return { ms * 1'000'000 }; }
};

}
