#include <iostream>
#include <random>
#include <thread>
#include "place.hpp"

place::place(int n, int x, int y) : n(n), x(x), y(y) { }

void place::request(int other_owner) {
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

int place::get_owner() {
    return owner;
}

void place::release() {
    owner = -1;
    free = true;
    channel.notify_all();
}

bool place::is_free() {
    return free;
}

std::mutex& place::get_mutex() {
    return mutex;
}

int place::get_x(){
    return x;
}

int place::get_y(){
    return y;
}
