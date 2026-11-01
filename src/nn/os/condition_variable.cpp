#include "nn/os/condition_variable.h"
#include <chrono>

namespace nn::os {

void InitializeConditionVariable(ConditionVariableType* cv) { (void)cv; }
void FinalizeConditionVariable(ConditionVariableType* cv) { (void)cv; }

void SignalConditionVariable(ConditionVariableType* cv)
{
    cv->handle.notify_one();
}

void BroadcastConditionVariable(ConditionVariableType* cv)
{
    cv->handle.notify_all();
}

void WaitConditionVariable(ConditionVariableType* cv, MutexType* mutex)
{
    // The game has already locked the mutex: wait() unlocks it while sleeping, relocks after.
    cv->handle.wait(mutex->handle);
}

ConditionVariableStatus TimedWaitConditionVariable(ConditionVariableType* cv, MutexType* mutex, TimeSpan timeout)
{
    if (cv->handle.wait_for(mutex->handle, std::chrono::nanoseconds(timeout.nanoSeconds)) == std::cv_status::timeout)
        return ConditionVariableStatus_Timeout;
    return ConditionVariableStatus_Success;
}

}
