#pragma once
#include <mutex>
#include <shared_mutex>

namespace nn::os
{
    // --- Types
    struct MutexType {
        std::recursive_mutex handle;
    };

    struct ReaderWriterLockType {
        std::shared_mutex handle;
    };

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
