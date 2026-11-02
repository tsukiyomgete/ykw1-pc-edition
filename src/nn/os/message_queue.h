#pragma once
//Files de messages : une boite aux lettres de nombres entre threads
#include <condition_variable>
#include <deque>
#include <mutex>
#include <cstddef>
#include <cstdint>

namespace nn::os
{
    // --- Types
    struct MessageQueueType {
        std::mutex lock;
        std::condition_variable notEmpty;
        std::condition_variable notFull;
        std::deque<uintptr_t> messages;
        size_t capacity = 0;
    };

    // --- Message queues
    void InitializeMessageQueue(MessageQueueType* queue, uintptr_t* buffer, size_t count);
    void FinalizeMessageQueue(MessageQueueType* queue);
    void SendMessageQueue(MessageQueueType* queue, uintptr_t data); //ajoute a la fin, attend si plein
    bool TrySendMessageQueue(MessageQueueType* queue, uintptr_t data);
    void JamMessageQueue(MessageQueueType* queue, uintptr_t data); //ajoute au DEBUT, attend si plein
    bool TryJamMessageQueue(MessageQueueType* queue, uintptr_t data);
    void ReceiveMessageQueue(uintptr_t* outData, MessageQueueType* queue); //attention : out en premier
    bool TryReceiveMessageQueue(uintptr_t* outData, MessageQueueType* queue);
}
