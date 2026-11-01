#include "nn/os/thread.h"
#include "nn/os/tls.h"
#include <atomic>
#include <chrono>

namespace nn::os {

thread_local ThreadType* g_CurrentThread = nullptr;
ThreadType g_MainThread;
std::atomic<uint64_t> g_NextThreadId{1};


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
    detail::RunTlsDestructors();   // clean this thread's TLS values (see tls.cpp)
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

}
