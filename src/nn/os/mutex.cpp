#include "nn/os/mutex.h"

namespace nn::os {

// Mutex
void InitializeMutex(MutexType* mutex, bool recursive, int lockLevel)
{
    (void)mutex; (void)recursive; (void)lockLevel;
}

void FinalizeMutex(MutexType* mutex)
{
    (void)mutex;
}

void LockMutex(MutexType* mutex)
{
    mutex->handle.lock();
}

bool TryLockMutex(MutexType* mutex)
{
    return mutex->handle.try_lock();
}

void UnlockMutex(MutexType* mutex)
{
    mutex->handle.unlock();
}


//Verrou lecteurs/ecrivains
void InitializeReaderWriterLock(ReaderWriterLockType* lock)
{
    (void) lock;
}

void FinalizeReaderWriterLock(ReaderWriterLockType* lock)
{
    (void) lock;
}

void AcquireWriteLock(ReaderWriterLockType* lock)
{
    lock->handle.lock();
}
void ReleaseWriteLock(ReaderWriterLockType* lock)
{
    lock->handle.unlock();
}
void AcquireReadLock(ReaderWriterLockType* lock)
{
    lock->handle.lock_shared();
}
void ReleaseReadLock(ReaderWriterLockType* lock)
{
    lock->handle.unlock_shared();
}

}
