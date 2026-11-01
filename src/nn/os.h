#pragma once
#include <mutex>
#include <thread>
#include <cstddef>
#include "nn/types.h"
#include <shared_mutex>

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

    struct ReaderWriterLockType {
        std::shared_mutex handle;
    };

    struct MutexType {
        std::recursive_mutex handle;
    };

    struct Tick {
        int64_t value = 0;
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
    void SetThreadName(ThreadType* thread, const char* name);
    const char* GetThreadNamePointer(const ThreadType* thread);
    int ChangeThreadPriority(ThreadType* thread, int priority);
    int GetThreadPriority(const ThreadType* thread);
    void SetThreadCoreMask(ThreadType* thread, int idealCore, uint64_t affinityMask);
    uint64_t GetThreadAvailableCoreMask();
    int GetCurrentCoreNumber();
    void GetCurrentStackInfo(uintptr_t* outStack, size_t* outSize);
    ThreadType* GetCurrentThread();
    uint64_t GetThreadId(const ThreadType* thread);
    
    
    // --- Tick
    Tick GetSystemTick();
    int64_t GetSystemTickFrequency();

    // --- Mutex
    void InitializeMutex(MutexType* mutex, bool recursive, int lockLevel);
    void FinalizeMutex(MutexType* mutex);
    void LockMutex(MutexType* mutex);
    bool TryLockMutex(MutexType* mutex);
    void UnlockMutex(MutexType* mutex);

    // --- Reader/writer lock
    void InitializeReaderWriterLock(ReaderWriterLockType* lock);
    void FinalizeReaderWriterLock(ReaderWriterLockType* lock);
    void AcquireWriteLock(ReaderWriterLockType* lock);
    void ReleaseWriteLock(ReaderWriterLockType* lock);
    void AcquireReadLock(ReaderWriterLockType* lock);
    void ReleaseReadLock(ReaderWriterLockType* lock);
}