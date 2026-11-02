#include "nn/os/event.h"
#include <chrono>

namespace nn::os {

//Reveille WaitAny a chaque fois qu'un event est signale
static std::mutex g_AnySignalLock;
static std::condition_variable g_AnySignal;

static void NotifyAny()
{
    std::lock_guard<std::mutex> guard(g_AnySignalLock);
    g_AnySignal.notify_all();
}

//Les 3 sortes d'events marchent pareil, donc on ecrit le code une seule fois
static void CoreInit(EventCore& e, bool signaled, EventClearMode mode)
{
    std::lock_guard<std::mutex> guard(e.lock);
    e.signaled = signaled;
    if (mode == EventClearMode_AutoClear)
    {
        e.autoClear = true;
    }
    else
    {
        e.autoClear = false;
    }
}

static void CoreSignal(EventCore& e)
{
    {
        std::lock_guard<std::mutex> guard(e.lock);
        e.signaled = true;
    }
    if (e.autoClear)
    {
        e.changed.notify_one(); //auto-clear : un seul thread peut passer
    }
    else
    {
        e.changed.notify_all(); //manuel : tout le monde passe
    }
    NotifyAny();
}

static void CoreWait(EventCore& e)
{
    std::unique_lock<std::mutex> guard(e.lock);
    while (!e.signaled)
    {
        e.changed.wait(guard);
    }
    if (e.autoClear)
    {
        e.signaled = false;
    }
}

static bool CoreTryWait(EventCore& e)
{
    std::lock_guard<std::mutex> guard(e.lock);
    if (!e.signaled)
    {
        return false;
    }
    if (e.autoClear)
    {
        e.signaled = false;
    }
    return true;
}

static bool CoreTimedWait(EventCore& e, TimeSpan timeout)
{
    std::unique_lock<std::mutex> guard(e.lock);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now() + std::chrono::nanoseconds(timeout.nanoSeconds);
    while (!e.signaled)
    {
        if (e.changed.wait_until(guard, end) == std::cv_status::timeout)
        {
            break;
        }
    }
    if (!e.signaled)
    {
        return false;
    }
    if (e.autoClear)
    {
        e.signaled = false;
    }
    return true;
}

static void CoreClear(EventCore& e)
{
    std::lock_guard<std::mutex> guard(e.lock);
    e.signaled = false;
}

static bool CoreIsSignaled(EventCore& e)
{
    std::lock_guard<std::mutex> guard(e.lock);
    return e.signaled;
}

//Events
void InitializeEvent(EventType* event, bool signaled, EventClearMode clearMode)
{
    CoreInit(event->core, signaled, clearMode);
}
void FinalizeEvent(EventType* event)
{
    (void)event;
}
void SignalEvent(EventType* event)
{
    CoreSignal(event->core);
}
void WaitEvent(EventType* event)
{
    CoreWait(event->core);
}
bool TryWaitEvent(EventType* event)
{
    return CoreTryWait(event->core);
}
bool TimedWaitEvent(EventType* event, TimeSpan timeout)
{
    return CoreTimedWait(event->core, timeout);
}
void ClearEvent(EventType* event)
{
    CoreClear(event->core);
}

//Light events
void InitializeLightEvent(LightEventType* event, bool signaled, EventClearMode clearMode)
{
    CoreInit(event->core, signaled, clearMode);
}
void FinalizeLightEvent(LightEventType* event)
{
    (void)event;
}
void SignalLightEvent(LightEventType* event)
{
    CoreSignal(event->core);
}
void WaitLightEvent(LightEventType* event)
{
    CoreWait(event->core);
}
bool TryWaitLightEvent(LightEventType* event)
{
    return CoreTryWait(event->core);
}
bool TimedWaitLightEvent(LightEventType* event, TimeSpan timeout)
{
    return CoreTimedWait(event->core, timeout);
}
void ClearLightEvent(LightEventType* event)
{
    CoreClear(event->core);
}

//System events
Result CreateSystemEvent(SystemEventType* event, EventClearMode clearMode, bool interProcess)
{
    (void)interProcess;
    CoreInit(event->core, false, clearMode);
    return Result{};
}
void DestroySystemEvent(SystemEventType* event)
{
    (void)event;
}
void SignalSystemEvent(SystemEventType* event)
{
    CoreSignal(event->core);
}
void WaitSystemEvent(SystemEventType* event)
{
    CoreWait(event->core);
}
bool TryWaitSystemEvent(SystemEventType* event)
{
    return CoreTryWait(event->core);
}
bool TimedWaitSystemEvent(SystemEventType* event, TimeSpan timeout)
{
    return CoreTimedWait(event->core, timeout);
}
void ClearSystemEvent(SystemEventType* event)
{
    CoreClear(event->core);
}

//Multi-wait : attendre qu'UN des events soit signale
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
    if (multiWait == nullptr)
    {
        return;
    }
    std::lock_guard<std::mutex> guard(multiWait->lock);
    for (size_t i = 0; i < multiWait->holders.size(); i++)
    {
        if (multiWait->holders[i] == holder)
        {
            multiWait->holders.erase(multiWait->holders.begin() + i);
            break;
        }
    }
    holder->owner = nullptr;
}

MultiWaitHolderType* WaitAny(MultiWaitType* multiWait)
{
    //Comme sur Switch, on ne remet pas l'event a zero : le jeu le fait lui-meme apres
    while (true)
    {
        {
            std::lock_guard<std::mutex> guard(multiWait->lock);
            for (size_t i = 0; i < multiWait->holders.size(); i++)
            {
                MultiWaitHolderType* holder = multiWait->holders[i];
                if (holder->event != nullptr && CoreIsSignaled(holder->event->core))
                {
                    return holder;
                }
            }
        }
        //Rien pour l'instant : on dort jusqu'au prochain signal (et on reverifie toutes les 10 ms)
        std::unique_lock<std::mutex> guard(g_AnySignalLock);
        g_AnySignal.wait_for(guard, std::chrono::milliseconds(10));
    }
}

}
