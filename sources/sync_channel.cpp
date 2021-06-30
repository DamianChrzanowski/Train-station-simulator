#include "sync_channel.hpp"

void sync_channel::wait() {
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock);
}

void sync_channel::notify_all() {
    std::unique_lock<std::mutex> lock(mutex);
    condition.notify_all();
}
