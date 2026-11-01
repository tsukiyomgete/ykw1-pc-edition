#pragma once
#include <mutex>
#include <shared_mutex>
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

    struct MutexType {
        std::recursive_mutex handle;
    };

    struct ReaderWriterLockType {
        std::shared_mutex handle;
    };

    struct Tick {
        int64_t value = 0;
    };

    struct MemoryInfo {
        size_t totalAvailableMemorySize = 0;
        size_t totalUsedMemorySize = 0;
        size_t totalMemoryHeapSize = 0;
        size_t allocatedMemoryHeapSize = 0;
        size_t totalThreadStackSize = 0;
        int threadCount = 0;
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

    // --- Tick
    Tick GetSystemTick();
    int64_t GetSystemTickFrequency();
    TimeSpan ConvertToTimeSpan(Tick tick);

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

    // --- Misc
    void GenerateRandomBytes(void* buffer, size_t size);
    Result SetMemoryHeapSize(size_t size);
    Result AllocateMemoryBlock(uintptr_t* outAddress, size_t size);
    void QueryMemoryInfo(MemoryInfo* info);
}