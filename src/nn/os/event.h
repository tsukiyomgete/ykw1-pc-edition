#pragma once
//Events (normaux, light, system) et multi-wait
#include <condition_variable>
#include <deque>
#include <mutex>
#include "nn/types.h"

namespace nn::os
{
    // --- Types
    enum EventClearMode {
        EventClearMode_ManualClear = 0,   
        EventClearMode_AutoClear = 1,     
    };

    //Moteur commun aux 3 sortes d'events
    struct EventCore {
        std::mutex lock;
        std::condition_variable changed;
        bool signaled = false;
        bool autoClear = false;
    };

    struct EventType       { EventCore core; };
    struct LightEventType  { EventCore core; };
    struct SystemEventType { EventCore core; };   

    struct MultiWaitType;
    struct MultiWaitHolderType {
        EventType* event = nullptr;
        MultiWaitType* owner = nullptr;
    };
    struct MultiWaitType {
        std::mutex lock;
        std::deque<MultiWaitHolderType*> holders;
    };

    // --- Events
    void InitializeEvent(EventType* event, bool signaled, EventClearMode clearMode);
    void FinalizeEvent(EventType* event);
    void SignalEvent(EventType* event);
    void WaitEvent(EventType* event);
    bool TryWaitEvent(EventType* event);
    bool TimedWaitEvent(EventType* event, TimeSpan timeout);
    void ClearEvent(EventType* event);

    // --- Light events (meme fonctionnement, plus legers sur Switch)
    void InitializeLightEvent(LightEventType* event, bool signaled, EventClearMode clearMode);
    void FinalizeLightEvent(LightEventType* event);
    void SignalLightEvent(LightEventType* event);
    void WaitLightEvent(LightEventType* event);
    bool TryWaitLightEvent(LightEventType* event);
    bool TimedWaitLightEvent(LightEventType* event, TimeSpan timeout);
    void ClearLightEvent(LightEventType* event);

    // --- System events (le jeu ne fait que les attendre)
    Result CreateSystemEvent(SystemEventType* event, EventClearMode clearMode, bool interProcess);
    void DestroySystemEvent(SystemEventType* event);
    void SignalSystemEvent(SystemEventType* event);
    void WaitSystemEvent(SystemEventType* event);
    bool TryWaitSystemEvent(SystemEventType* event);
    bool TimedWaitSystemEvent(SystemEventType* event, TimeSpan timeout);
    void ClearSystemEvent(SystemEventType* event);

    // --- Multi-wait (attendre qu'UN des events soit signale)
    void InitializeMultiWait(MultiWaitType* multiWait);
    void FinalizeMultiWait(MultiWaitType* multiWait);
    void InitializeMultiWaitHolder(MultiWaitHolderType* holder, EventType* event);
    void FinalizeMultiWaitHolder(MultiWaitHolderType* holder);
    void LinkMultiWaitHolder(MultiWaitType* multiWait, MultiWaitHolderType* holder);
    void UnlinkMultiWaitHolder(MultiWaitHolderType* holder);
    MultiWaitHolderType* WaitAny(MultiWaitType* multiWait);
}
