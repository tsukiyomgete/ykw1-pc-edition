#include "nn/mem.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace nn::mem {

//Juste avant chaque bloc donne au jeu, on cache cette petite fiche
struct BlockHeader {
    void* raw;    //l'adresse renvoyee par malloc (pour free)
    size_t size;  //la taille demandee par le jeu
};

StandardAllocator::StandardAllocator()
{
    totalSize = 0;
    usedSize = 0;
}

void StandardAllocator::Initialize(void* address, size_t size)
{
    (void)address; //on n'utilise pas le bloc du jeu, malloc s'en occupe
    totalSize = size;
    usedSize = 0;
}

void StandardAllocator::Initialize(void* address, size_t size, bool enableCache)
{
    (void)enableCache;
    Initialize(address, size);
}

void StandardAllocator::Finalize()
{
    totalSize = 0;
    usedSize = 0;
}

void* StandardAllocator::Allocate(size_t size)
{
    return Allocate(size, 16);
}

void* StandardAllocator::Allocate(size_t size, size_t alignment)
{
    if (alignment < 16)
    {
        alignment = 16;
    }
    //On demande un peu plus pour avoir la place de la fiche et pour aligner
    void* raw = std::malloc(size + alignment + sizeof(BlockHeader));
    if (raw == nullptr)
    {
        return nullptr;
    }
    uintptr_t start = (uintptr_t)raw + sizeof(BlockHeader);
    uintptr_t aligned = (start + alignment - 1) / alignment * alignment;

    BlockHeader* header = (BlockHeader*)(aligned - sizeof(BlockHeader));
    header->raw = raw;
    header->size = size;
    usedSize = usedSize + size;
    return (void*)aligned;
}

void StandardAllocator::Free(void* address)
{
    if (address == nullptr)
    {
        return;
    }
    BlockHeader* header = (BlockHeader*)((uintptr_t)address - sizeof(BlockHeader));
    usedSize = usedSize - header->size;
    std::free(header->raw);
}

void* StandardAllocator::Reallocate(void* address, size_t newSize)
{
    if (address == nullptr)
    {
        return Allocate(newSize);
    }
    BlockHeader* header = (BlockHeader*)((uintptr_t)address - sizeof(BlockHeader));
    void* newAddress = Allocate(newSize);
    if (newAddress == nullptr)
    {
        return nullptr;
    }
    size_t copySize = header->size;
    if (newSize < copySize)
    {
        copySize = newSize;
    }
    std::memcpy(newAddress, address, copySize);
    Free(address);
    return newAddress;
}

size_t StandardAllocator::GetTotalFreeSize() const
{
    if (usedSize > totalSize)
    {
        return 0;
    }
    return totalSize - usedSize;
}

}
