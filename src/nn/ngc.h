#pragma once
#include <cstddef>
#include <cstdint>
#include "nn/types.h"

//Filtre des gros mots (pour les surnoms) : sur PC, on accepte tout
namespace nn::ngc
{
    class ProfanityFilter {
    public:
        ProfanityFilter();
        ~ProfanityFilter();
        Result Initialize(void* workMemory, size_t workMemorySize);
        void   Finalize();
        Result CheckProfanityWords(uint32_t* outResults, uint32_t patterns, const char** words, size_t wordCount);
    };
}
