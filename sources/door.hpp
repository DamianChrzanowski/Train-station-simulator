#ifndef DOOR_H
#define DOOR_H

#include <mutex>
#include "sync_channel.hpp"

class door
{
public:
    door(int x, int y);
    void request(int other_owner);
    void release();
    bool is_free();
    bool are_open();
    void set_open(bool open);
    int get_owner();
    int get_x();
    int get_y();
    std::mutex& get_mutex();

private:
    int x;
    int y;
    int owner{ -1 };
    bool free{ true };
    bool open{ false };
    
    std::mutex mutex;
    sync_channel channel;
};

#endif