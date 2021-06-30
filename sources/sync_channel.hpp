#ifndef SYNC_CHANNEL_H
#define SYNC_CHANNEL_H

#include <mutex>
#include <condition_variable>

class sync_channel {
public:
    void wait();
    void notify_all();

private:
    std::mutex mutex;
    std::condition_variable condition;
};

#endif
