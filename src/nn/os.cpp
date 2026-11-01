#include "nn/os.h"
#include <chrono>

namespace nn::os {

void InitializeMutex(MutexType* mutex, bool recursive, int lockLevel)
{
    // std::recursive_mutex is ready as soon as it exists: nothing to do.
    (void)mutex; (void)recursive; (void)lockLevel;
}

void FinalizeMutex(MutexType* mutex)
{
    // Nothing to free: the mutex is destroyed with its MutexType.
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


Result CreateThread(ThreadType* thread, ThreadFunction function, void* argument,
void* stack, size_t stackSize, int priority)
{
    thread->function = function;
    thread->argument = argument;
    (void)stack; (void)stackSize; (void)priority;
    return Result{};
}

void StartThread(ThreadType* thread)
{
    thread->handle = std::thread(thread->function, thread->argument);
}
void WaitThread(ThreadType* thread)
{
    thread->handle.join();
}
void DestroyThread(ThreadType* thread)
{
    if (thread->handle.joinable())
    {
        thread->handle.join();
    }
}
void SleepThread(TimeSpan time)
{
    std::this_thread::sleep_for(std::chrono::nanoseconds(time.nanoSeconds));
}

}