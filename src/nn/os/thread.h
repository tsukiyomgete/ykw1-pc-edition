#pragma once
#include <thread>
#include <cstddef>
#include <cstdint>
#include "nn/types.h"

namespace nn::os
{
    // --- Types
    using ThreadFunction = void (*)(void* argument);

    struct ThreadType {
        std::thread handle;
        ThreadFunction function = nullptr;
        void* argument = nullptr;
        const char* name = "";
        int priority = 0;
        uint64_t id = 0;
    };

    // --- Threads
    Result CreateThread(ThreadType* thread, ThreadFunction function, void* argument,
                        void* stack, size_t stackSize, int priority);
    Result CreateThread(ThreadType* thread, ThreadFunction function, void* argument,
                        void* stack, size_t stackSize, int priority, int idealCore);
    void StartThread(ThreadType* thread);
    void WaitThread(ThreadType* thread);
    void DestroyThread(ThreadType* thread);
    void SleepThread(TimeSpan time);
    void YieldThread();
    ThreadType* GetCurrentThread();
    uint64_t GetThreadId(const ThreadType* thread);
    void SetThreadName(ThreadType* thread, const char* name);
    const char* GetThreadNamePointer(const ThreadType* thread);
    int ChangeThreadPriority(ThreadType* thread, int priority);
    int GetThreadPriority(const ThreadType* thread);
    void SetThreadCoreMask(ThreadType* thread, int idealCore, uint64_t affinityMask);
    uint64_t GetThreadAvailableCoreMask();
    int GetCurrentCoreNumber();
    void GetCurrentStackInfo(uintptr_t* outStack, size_t* outSize);
}
