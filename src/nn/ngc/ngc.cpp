#include "nn/ngc.h"

namespace nn::ngc {

ProfanityFilter::ProfanityFilter()
{
}

ProfanityFilter::~ProfanityFilter()
{
}

Result ProfanityFilter::Initialize(void* workMemory, size_t workMemorySize)
{
    (void)workMemory;
    (void)workMemorySize;
    return Result{};
}

void ProfanityFilter::Finalize()
{
}

Result ProfanityFilter::CheckProfanityWords(uint32_t* outResults, uint32_t patterns, const char** words, size_t wordCount)
{
    (void)patterns;
    (void)words;
    //0 = aucun gros mot trouve
    for (size_t i = 0; i < wordCount; i++)
    {
        outResults[i] = 0;
    }
    return Result{};
}

}
