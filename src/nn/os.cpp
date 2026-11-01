#include "nn/os.h"
#include <chrono>
#include <atomic>       
#include <cstdlib>
#include <random>

namespace nn::os {

thread_local ThreadType* g_CurrentThread = nullptr;  
ThreadType g_MainThread;
std::atomic<uint64_t> g_NextThreadId{1};  


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


// Shared Mutex
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



// Thread

Result CreateThread(ThreadType* thread, ThreadFunction function, void* argument,
void* stack, size_t stackSize, int priority)
{
    thread->function = function;
    thread->argument = argument;
    thread->id = g_NextThreadId++;
    (void)stack; (void)stackSize; (void)priority;
    return Result{};
}

Result CreateThread(ThreadType* thread, ThreadFunction function, void* argument,
                    void* stack, size_t stackSize, int priority, int idealCore)
{
    (void)idealCore;
    return CreateThread(thread, function, argument, stack, stackSize, priority);
}

void ThreadEntry(ThreadType* thread)
{
    g_CurrentThread = thread;                
    thread->function(thread->argument);  
}

ThreadType* GetCurrentThread()
{
    if (g_CurrentThread == nullptr)
    {
        return &g_MainThread; 
    }           
    return g_CurrentThread;
}

void StartThread(ThreadType* thread)
{
    thread->handle = std::thread(ThreadEntry, thread);
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

void SetThreadName(ThreadType* thread, const char* name)
{
    thread->name = name;
}

int ChangeThreadPriority(ThreadType* thread, int priority)
{
    int old = thread->priority;
    thread->priority = priority;
    return old;
}

const char* GetThreadNamePointer(const ThreadType* thread)
{
    return thread->name;
}

int GetThreadPriority(const ThreadType* thread)
{
    return thread->priority;
}
uint64_t GetThreadId(const ThreadType* thread)
{
    return thread->id;
}

void YieldThread()
{
    std::this_thread::yield();
}

int GetCurrentCoreNumber()
{
    return 0;
}

void SetThreadCoreMask(ThreadType* thread, int idealCore, uint64_t affinityMask)
{
    // Windows chooses the cores itself: nothing to do.
    (void)thread; (void)idealCore; (void)affinityMask;
}

uint64_t GetThreadAvailableCoreMask()
{
    return 0b1111;   // 4 bits set = cores 0, 1, 2 and 3 are available
}

void GetCurrentStackInfo(uintptr_t* outStack, size_t* outSize)
{
    *outStack = 0;   // write 0 into the place outStack points to
    *outSize = 0;
}

// Tick

Tick GetSystemTick()
{
    std::chrono::steady_clock::duration now = std::chrono::steady_clock::now().time_since_epoch();
    int64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();

    Tick result;                 
    result.value = ns * 12 / 625;  
    return result;
}

int64_t GetSystemTickFrequency()
{
    return 19200000;
}

TimeSpan ConvertToTimeSpan(Tick tick)
{
    TimeSpan result;
    result.nanoSeconds = (tick.value * 625)/12;
    return result;
}

void GenerateRandomBytes(void* buffer, size_t size)
{
    std::random_device rng;                           // the PC's random source
    uint8_t* bytes = static_cast<uint8_t*>(buffer);   // see buffer as a list of bytes
    for (size_t i = 0; i < size; i++)
    {
        bytes[i] = static_cast<uint8_t>(rng() % 256); // keep only what fits in one byte
    }
}

Result SetMemoryHeapSize(size_t size)
{
    // On PC, memory is taken when needed: nothing to reserve.
    (void)size;
    return Result{};
}

Result AllocateMemoryBlock(uintptr_t* outAddress, size_t size)
{
    void* block = malloc(size);
    *outAddress = reinterpret_cast<uintptr_t>(block);  // give the address back as a number
    return Result{};
}

void QueryMemoryInfo(MemoryInfo* info)
{
    *info = MemoryInfo{};   // everything set to 0
}

}