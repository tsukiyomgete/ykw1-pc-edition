#include "nn/os/message_queue.h"

namespace nn::os {

void InitializeMessageQueue(MessageQueueType* queue, uintptr_t* buffer, size_t count)
{
    (void)buffer;   // the game's buffer is not needed: std::deque stores the messages
    std::lock_guard<std::mutex> guard(queue->lock);
    queue->messages.clear();
    queue->capacity = count;
}

void FinalizeMessageQueue(MessageQueueType* queue)
{
    std::lock_guard<std::mutex> guard(queue->lock);
    queue->messages.clear();
}

static void Put(MessageQueueType* queue, uintptr_t data, bool front)
{
    std::unique_lock<std::mutex> guard(queue->lock);
    queue->notFull.wait(guard, [&] { return queue->messages.size() < queue->capacity; });
    if (front) queue->messages.push_front(data);
    else       queue->messages.push_back(data);
    queue->notEmpty.notify_one();
}

static bool TryPut(MessageQueueType* queue, uintptr_t data, bool front)
{
    std::lock_guard<std::mutex> guard(queue->lock);
    if (queue->messages.size() >= queue->capacity) return false;
    if (front) queue->messages.push_front(data);
    else       queue->messages.push_back(data);
    queue->notEmpty.notify_one();
    return true;
}

void SendMessageQueue(MessageQueueType* queue, uintptr_t data) { Put(queue, data, false); }
bool TrySendMessageQueue(MessageQueueType* queue, uintptr_t data) { return TryPut(queue, data, false); }
void JamMessageQueue(MessageQueueType* queue, uintptr_t data) { Put(queue, data, true); }
bool TryJamMessageQueue(MessageQueueType* queue, uintptr_t data) { return TryPut(queue, data, true); }

void ReceiveMessageQueue(uintptr_t* outData, MessageQueueType* queue)
{
    std::unique_lock<std::mutex> guard(queue->lock);
    queue->notEmpty.wait(guard, [&] { return !queue->messages.empty(); });
    *outData = queue->messages.front();
    queue->messages.pop_front();
    queue->notFull.notify_one();
}

bool TryReceiveMessageQueue(uintptr_t* outData, MessageQueueType* queue)
{
    std::lock_guard<std::mutex> guard(queue->lock);
    if (queue->messages.empty()) return false;
    *outData = queue->messages.front();
    queue->messages.pop_front();
    queue->notFull.notify_one();
    return true;
}

}
