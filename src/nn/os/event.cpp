#include "nn/os/event.h"
#include <chrono>

namespace nn::os {

namespace {

// Every Signal wakes WaitAny, which then checks its events again.
std::mutex g_AnySignalLock;
std::condition_variable g_AnySignal;

void NotifyAny()
{
    std::lock_guard<std::mutex> guard(g_AnySignalLock);
    g_AnySignal.notify_all();
}

// --- The shared event engine
void CoreInit(EventCore& e, bool signaled, EventClearMode mode)
{
    std::lock_guard<std::mutex> guard(e.lock);
    e.signaled = signaled;
    e.autoClear = (mode == EventClearMode_AutoClear);
}

void CoreSignal(EventCore& e)
{
    {
        std::lock_guard<std::mutex> guard(e.lock);
        e.signaled = true;
    }
    if (e.autoClear) e.changed.notify_one();   // auto-clear: only one waiter may pass
    else             e.changed.notify_all();   // manual: everybody passes
    NotifyAny();
}

void CoreWait(EventCore& e)
{
    std::unique_lock<std::mutex> guard(e.lock);
    e.changed.wait(guard, [&] { return e.signaled; });
    if (e.autoClear) e.signaled = false;
}

bool CoreTryWait(EventCore& e)
{
    std::lock_guard<std::mutex> guard(e.lock);
    if (!e.signaled) return false;
    if (e.autoClear) e.signaled = false;
    return true;
}

bool CoreTimedWait(EventCore& e, TimeSpan timeout)
{
    std::unique_lock<std::mutex> guard(e.lock);
    if (!e.changed.wait_for(guard, std::chrono::nanoseconds(timeout.nanoSeconds), [&] { return e.signaled; }))
        return false;
    if (e.autoClear) e.signaled = false;
    return true;
}

void CoreClear(EventCore& e)
{
    std::lock_guard<std::mutex> guard(e.lock);
    e.signaled = false;
}

bool CoreIsSignaled(EventCore& e)
{
    std::lock_guard<std::mutex> guard(e.lock);
    return e.signaled;
}

} // anonymous namespace

// ===== Events
void InitializeEvent(EventType* event, bool signaled, EventClearMode clearMode) { CoreInit(event->core, signaled, clearMode); }
void FinalizeEvent(EventType* event) { (void)event; }
void SignalEvent(EventType* event) { CoreSignal(event->core); }
void WaitEvent(EventType* event) { CoreWait(event->core); }
bool TryWaitEvent(EventType* event) { return CoreTryWait(event->core); }
bool TimedWaitEvent(EventType* event, TimeSpan timeout) { return CoreTimedWait(event->core, timeout); }
void ClearEvent(EventType* event) { CoreClear(event->core); }

// ===== Light events
void InitializeLightEvent(LightEventType* event, bool signaled, EventClearMode clearMode) { CoreInit(event->core, signaled, clearMode); }
void FinalizeLightEvent(LightEventType* event) { (void)event; }
void SignalLightEvent(LightEventType* event) { CoreSignal(event->core); }
void WaitLightEvent(LightEventType* event) { CoreWait(event->core); }
bool TryWaitLightEvent(LightEventType* event) { return CoreTryWait(event->core); }
bool TimedWaitLightEvent(LightEventType* event, TimeSpan timeout) { return CoreTimedWait(event->core, timeout); }
void ClearLightEvent(LightEventType* event) { CoreClear(event->core); }

// ===== System events
Result CreateSystemEvent(SystemEventType* event, EventClearMode clearMode, bool interProcess)
{
    (void)interProcess;
    CoreInit(event->core, false, clearMode);
    return Result{};
}
void DestroySystemEvent(SystemEventType* event) { (void)event; }
void SignalSystemEvent(SystemEventType* event) { CoreSignal(event->core); }
void WaitSystemEvent(SystemEventType* event) { CoreWait(event->core); }
bool TryWaitSystemEvent(SystemEventType* event) { return CoreTryWait(event->core); }
bool TimedWaitSystemEvent(SystemEventType* event, TimeSpan timeout) { return CoreTimedWait(event->core, timeout); }
void ClearSystemEvent(SystemEventType* event) { CoreClear(event->core); }

// ===== Multi-wait
void InitializeMultiWait(MultiWaitType* multiWait)
{
    std::lock_guard<std::mutex> guard(multiWait->lock);
    multiWait->holders.clear();
}

void FinalizeMultiWait(MultiWaitType* multiWait)
{
    std::lock_guard<std::mutex> guard(multiWait->lock);
    multiWait->holders.clear();
}

void InitializeMultiWaitHolder(MultiWaitHolderType* holder, EventType* event)
{
    holder->event = event;
    holder->owner = nullptr;
}

void FinalizeMultiWaitHolder(MultiWaitHolderType* holder)
{
    holder->event = nullptr;
}

void LinkMultiWaitHolder(MultiWaitType* multiWait, MultiWaitHolderType* holder)
{
    std::lock_guard<std::mutex> guard(multiWait->lock);
    multiWait->holders.push_back(holder);
    holder->owner = multiWait;
}

void UnlinkMultiWaitHolder(MultiWaitHolderType* holder)
{
    MultiWaitType* multiWait = holder->owner;
    if (multiWait == nullptr) return;
    std::lock_guard<std::mutex> guard(multiWait->lock);
    for (auto it = multiWait->holders.begin(); it != multiWait->holders.end(); ++it)
    {
        if (*it == holder) { multiWait->holders.erase(it); break; }
    }
    holder->owner = nullptr;
}

MultiWaitHolderType* WaitAny(MultiWaitType* multiWait)
{
    // Returns the first holder whose event is signaled. Like on Switch, it does NOT clear the
    // event: the game calls TryWaitEvent / ClearEvent on it afterwards.
    while (true)
    {
        {
            std::lock_guard<std::mutex> guard(multiWait->lock);
            for (MultiWaitHolderType* holder : multiWait->holders)
            {
                if (holder->event != nullptr && CoreIsSignaled(holder->event->core))
                    return holder;
            }
        }
        // Nothing yet: sleep until any event is signaled (re-check every 10 ms just in case).
        std::unique_lock<std::mutex> guard(g_AnySignalLock);
        g_AnySignal.wait_for(guard, std::chrono::milliseconds(10));
    }
}

}
