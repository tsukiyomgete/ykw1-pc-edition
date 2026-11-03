#pragma once
#include <cstddef>

namespace nn::mem
{
    //L'allocateur principal du jeu. Sur Switch il gere un bloc de memoire donne par le jeu,
    //sur PC on passe simplement par malloc et free.
    class StandardAllocator {
    public:
        StandardAllocator();

        void   Initialize(void* address, size_t size);
        void   Initialize(void* address, size_t size, bool enableCache);
        void   Finalize();
        void*  Allocate(size_t size);
        void*  Allocate(size_t size, size_t alignment);
        void*  Reallocate(void* address, size_t newSize);
        void   Free(void* address);
        size_t GetTotalFreeSize() const;

    private:
        size_t totalSize;
        size_t usedSize;
    };
}
