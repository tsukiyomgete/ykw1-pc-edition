#include "nn/os/tls.h"
#include <array>
#include <cstddef>
#include <mutex>

namespace nn::os {

namespace {

constexpr size_t TlsSlotCount = 64;
std::mutex g_TlsLock;
std::array<bool, TlsSlotCount> g_TlsUsed{};
std::array<TlsDestructor, TlsSlotCount> g_TlsDestructors{};
thread_local std::array<uintptr_t, TlsSlotCount> t_TlsValues{};

} // anonymous namespace

Result AllocateTlsSlot(TlsSlot* outSlot, TlsDestructor destructor)
{
    std::lock_guard<std::mutex> guard(g_TlsLock);
    for (uint32_t i = 0; i < TlsSlotCount; i++)
    {
        if (!g_TlsUsed[i])
        {
            g_TlsUsed[i] = true;
            g_TlsDestructors[i] = destructor;
            outSlot->index = i;
            return Result{};
        }
    }
    return Result{ 1 };   // no free slot left
}

Result SdkAllocateTlsSlot(TlsSlot* outSlot, TlsDestructor destructor)
{
    return AllocateTlsSlot(outSlot, destructor);
}

void FreeTlsSlot(TlsSlot slot)
{
    std::lock_guard<std::mutex> guard(g_TlsLock);
    g_TlsUsed[slot.index] = false;
    g_TlsDestructors[slot.index] = nullptr;
}

void SetTlsValue(TlsSlot slot, uintptr_t value)
{
    t_TlsValues[slot.index] = value;
}

uintptr_t GetTlsValue(TlsSlot slot)
{
    return t_TlsValues[slot.index];
}

void detail::RunTlsDestructors()
{
    for (size_t i = 0; i < TlsSlotCount; i++)
    {
        TlsDestructor destructor;
        {
            std::lock_guard<std::mutex> guard(g_TlsLock);
            destructor = g_TlsDestructors[i];
        }
        if (destructor != nullptr && t_TlsValues[i] != 0)
            destructor(t_TlsValues[i]);
        t_TlsValues[i] = 0;
    }
}

}
