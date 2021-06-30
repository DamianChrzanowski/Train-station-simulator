#include "door.hpp"

door::door(int x, int y): x(x), y(y) {}

void door::request(int other_owner) {
    while (owner != other_owner) {
        if (free) {
            free = false;
            owner = other_owner;
        }
        else {
            channel.wait();
        }
    }
}

int door::get_owner() {
    return owner;
}

void door::release() {
    owner = -1;
    free = true;
    channel.notify_all();
}

bool door::is_free() {
    return free;
}

std::mutex& door::get_mutex() {
    return mutex;
}

int door::get_x(){
    return x;
}

int door::get_y(){
    return y;
}

bool door::are_open()
{
    return open;
}

void door::set_open(bool open)
{
    this->open = open;
}