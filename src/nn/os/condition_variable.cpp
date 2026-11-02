#include "nn/os/condition_variable.h"
#include <chrono>

namespace nn::os {

void InitializeConditionVariable(ConditionVariableType* cv)
{
    (void)cv;
}

void FinalizeConditionVariable(ConditionVariableType* cv)
{
    (void)cv;
}

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
    //Le jeu a deja verrouille le mutex : wait() le relache pendant l'attente puis le reprend
    cv->handle.wait(mutex->handle);
}

ConditionVariableStatus TimedWaitConditionVariable(ConditionVariableType* cv, MutexType* mutex, TimeSpan timeout)
{
    std::cv_status status = cv->handle.wait_for(mutex->handle, std::chrono::nanoseconds(timeout.nanoSeconds));
    if (status == std::cv_status::timeout)
    {
        return ConditionVariableStatus_Timeout;
    }
    return ConditionVariableStatus_Success;
}

}
