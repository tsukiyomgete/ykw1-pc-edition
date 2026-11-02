#include "nn/os/message_queue.h"

namespace nn::os {

void InitializeMessageQueue(MessageQueueType* queue, uintptr_t* buffer, size_t count)
{
    (void)buffer; //pas besoin du tableau du jeu, la deque garde les messages
    std::lock_guard<std::mutex> guard(queue->lock);
    queue->messages.clear();
    queue->capacity = count;
}

void FinalizeMessageQueue(MessageQueueType* queue)
{
    std::lock_guard<std::mutex> guard(queue->lock);
    queue->messages.clear();
}

//Ajoute un message, en attendant s'il n'y a plus de place
static void Put(MessageQueueType* queue, uintptr_t data, bool front)
{
    std::unique_lock<std::mutex> guard(queue->lock);
    while (queue->messages.size() >= queue->capacity)
    {
        queue->notFull.wait(guard);
    }
    if (front)
    {
        queue->messages.push_front(data);
    }
    else
    {
        queue->messages.push_back(data);
    }
    queue->notEmpty.notify_one();
}

//Pareil mais sans attendre : renvoie false si c'est plein
static bool TryPut(MessageQueueType* queue, uintptr_t data, bool front)
{
    std::lock_guard<std::mutex> guard(queue->lock);
    if (queue->messages.size() >= queue->capacity)
    {
        return false;
    }
    if (front)
    {
        queue->messages.push_front(data);
    }
    else
    {
        queue->messages.push_back(data);
    }
    queue->notEmpty.notify_one();
    return true;
}

void SendMessageQueue(MessageQueueType* queue, uintptr_t data)
{
    Put(queue, data, false);
}

bool TrySendMessageQueue(MessageQueueType* queue, uintptr_t data)
{
    return TryPut(queue, data, false);
}

void JamMessageQueue(MessageQueueType* queue, uintptr_t data)
{
    Put(queue, data, true);
}

bool TryJamMessageQueue(MessageQueueType* queue, uintptr_t data)
{
    return TryPut(queue, data, true);
}

void ReceiveMessageQueue(uintptr_t* outData, MessageQueueType* queue)
{
    std::unique_lock<std::mutex> guard(queue->lock);
    while (queue->messages.empty())
    {
        queue->notEmpty.wait(guard);
    }
    *outData = queue->messages.front();
    queue->messages.pop_front();
    queue->notFull.notify_one();
}

bool TryReceiveMessageQueue(uintptr_t* outData, MessageQueueType* queue)
{
    std::lock_guard<std::mutex> guard(queue->lock);
    if (queue->messages.empty())
    {
        return false;
    }
    *outData = queue->messages.front();
    queue->messages.pop_front();
    queue->notFull.notify_one();
    return true;
}

}
