#ifndef PLACE_H
#define PLACE_H

#include <mutex>
#include "sync_channel.hpp"

class place
{
public:
    place(int n, int x, int y);

    void request(int other_owner);
    void release();
    int get_x();
    int get_y();
    bool is_free();
    int get_owner();
    std::mutex& get_mutex();

private:
    int n;
    int x;
    int y;
    int owner {-1};
    bool free {true};
    std::mutex mutex;
    sync_channel channel;
    std::mt19937 rng{ std::random_device{}() };
};

#endif
