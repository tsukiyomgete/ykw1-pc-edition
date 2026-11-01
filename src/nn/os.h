#pragma once
#include <mutex>
#include <thread>
#include <cstddef>
#include "nn/types.h"

namespace nn::os
{
    // --- Types
    using ThreadFunction = void (*)(void* argument);

    struct ThreadType {
        std::thread handle;
        ThreadFunction function = nullptr;
        void* argument = nullptr;
    };

    struct MutexType {
        std::recursive_mutex handle;
    };

    struct Tick {
        int64_t value = 0;
    };

    
    Result CreateThread(ThreadType* thread, ThreadFunction function, void* argument,
    void* stack, size_t stackSize, int priority);
    void StartThread(ThreadType* thread);
    void WaitThread(ThreadType* thread);
    void DestroyThread(ThreadType* thread);
    void SleepThread(TimeSpan time);

    // --- Tick
    Tick GetSystemTick();
    int64_t GetSystemTickFrequency();

    // --- Mutex
    void InitializeMutex(MutexType* mutex, bool recursive, int lockLevel);
    void FinalizeMutex(MutexType* mutex);
    void LockMutex(MutexType* mutex);
    bool TryLockMutex(MutexType* mutex);
    void UnlockMutex(MutexType* mutex);
}