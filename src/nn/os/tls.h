#pragma once
// Thread local storage: one value per thread for each slot.
#include <cstdint>
#include "nn/types.h"

namespace nn::os
{
    // --- Types
    struct TlsSlot {
        uint32_t index = 0;
    };
    using TlsDestructor = void (*)(uintptr_t value);

    // --- Thread local storage
    Result AllocateTlsSlot(TlsSlot* outSlot, TlsDestructor destructor);
    Result SdkAllocateTlsSlot(TlsSlot* outSlot, TlsDestructor destructor);
    void FreeTlsSlot(TlsSlot slot);
    void SetTlsValue(TlsSlot slot, uintptr_t value);
    uintptr_t GetTlsValue(TlsSlot slot);

    namespace detail
    {
        // Called by ThreadEntry when a thread ends: runs the TLS destructors.
        void RunTlsDestructors();
    }
}
