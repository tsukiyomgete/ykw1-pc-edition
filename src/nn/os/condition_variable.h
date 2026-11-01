#pragma once
#include <condition_variable>
#include "nn/types.h"
#include "nn/os/mutex.h"

namespace nn::os
{
    // --- Types
    enum ConditionVariableStatus {
        ConditionVariableStatus_Timeout = 0,
        ConditionVariableStatus_Success = 1,
    };

    struct ConditionVariableType {
        std::condition_variable_any handle;   // "_any": works with our recursive_mutex
    };

    // --- Condition variables
    void InitializeConditionVariable(ConditionVariableType* cv);
    void FinalizeConditionVariable(ConditionVariableType* cv);
    void SignalConditionVariable(ConditionVariableType* cv);
    void BroadcastConditionVariable(ConditionVariableType* cv);
    void WaitConditionVariable(ConditionVariableType* cv, MutexType* mutex);
    ConditionVariableStatus TimedWaitConditionVariable(ConditionVariableType* cv, MutexType* mutex, TimeSpan timeout);
}
