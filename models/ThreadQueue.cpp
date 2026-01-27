#include "ThreadQueue.h"

bool ThreadSafeQueue::try_pop(GamePacket& packet) {
    std::lock_guard<std::mutex> lock(mtx);
    if (queue.empty()) return false;

    packet = queue.front();
    queue.pop();
    return true;
}

void ThreadSafeQueue::push(const GamePacket& packet) {
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(packet);
    cv.notify_one();
}

bool ThreadSafeQueue::pop(GamePacket& packet) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return !queue.empty() || shutdown; });

    if (shutdown && queue.empty()) return false;

    packet = queue.front();
    queue.pop();
    return true;
}

void ThreadSafeQueue::shutdown_queue() {
    shutdown = true;
    cv.notify_all();
}
