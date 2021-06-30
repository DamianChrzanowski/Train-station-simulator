#ifndef TICKET_MACHINE_H
#define TICKET_MACHINE_H

#include <mutex>
#include "sync_channel.hpp"

class ticket_machine
{
public:
    ticket_machine(int x, int y);
    void request(int other_owner);
    void release();
    int get_x();
    int get_y();
    bool is_free();
    int get_owner();
    std::mutex& get_mutex();

private:
    int x;
    int y;
    int owner {-1};
    bool free {true};
    
    std::mutex mutex;
    sync_channel channel;
};

#endif