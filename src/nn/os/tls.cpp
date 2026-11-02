#include "nn/os/tls.h"
#include <cstddef>
#include <mutex>

namespace nn::os {

//64 cases ; chaque thread a sa propre copie des valeurs (thread_local)
const int TLS_SLOT_COUNT = 64;
static std::mutex g_TlsLock;
static bool g_TlsUsed[TLS_SLOT_COUNT] = {};
static TlsDestructor g_TlsDestructors[TLS_SLOT_COUNT] = {};
thread_local uintptr_t t_TlsValues[TLS_SLOT_COUNT] = {};

Result AllocateTlsSlot(TlsSlot* outSlot, TlsDestructor destructor)
{
    std::lock_guard<std::mutex> guard(g_TlsLock);
    for (int i = 0; i < TLS_SLOT_COUNT; i++)
    {
        if (!g_TlsUsed[i])
        {
            g_TlsUsed[i] = true;
            g_TlsDestructors[i] = destructor;
            outSlot->index = i;
            return Result{};
        }
    }
    return Result{ 1 }; //plus de case libre
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
    for (int i = 0; i < TLS_SLOT_COUNT; i++)
    {
        TlsDestructor destructor;
        {
            std::lock_guard<std::mutex> guard(g_TlsLock);
            destructor = g_TlsDestructors[i];
        }
        if (destructor != nullptr && t_TlsValues[i] != 0)
        {
            destructor(t_TlsValues[i]);
        }
        t_TlsValues[i] = 0;
    }
}

}
