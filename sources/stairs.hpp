#ifndef STAIRS_H
#define STAIRS_H

#include <mutex>
#include "sync_channel.hpp"

class stairs
{
public:
    stairs (int x_top, int y_top, int x_down, int y_down);
    void request(int other_owner);
    void release();
    int get_x_top();
    int get_y_top();
    int get_x_down();
    int get_y_down();
    bool is_free();
    int get_owner();
    std::mutex& get_mutex();

private:
    int x_top;
    int y_top;
    int x_down;
    int y_down;
    int owner {-1};
    bool free {true};
    
    std::mutex mutex;
    sync_channel channel;
};

#endif