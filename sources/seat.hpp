#ifndef SEAT_H
#define SEAT_H

#include <mutex>
#include "sync_channel.hpp"

class seat
{
public:
    void request(int other_owner);
    void release();
    bool is_free();
    int get_owner();
    std::mutex& get_mutex();

private:
    int owner;
    bool free;
    
    std::mutex mutex;
    sync_channel channel;
    std::mt19937 rng{ std::random_device{}() };
};

#endif