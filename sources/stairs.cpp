#include "stairs.hpp"

stairs::stairs(int x_top, int y_top, int x_down, int y_down) : x_top(x_top), y_top(y_top), x_down(x_down), y_down(y_down) {}

void stairs::request(int other_owner) {
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

int stairs::get_owner() {
    return owner;
}

void stairs::release() {
    owner = -1;
    free = true;
    channel.notify_all();
}

bool stairs::is_free() {
    return free;
}

std::mutex& stairs::get_mutex() {
    return mutex;
}

int stairs::get_x_top(){
    return x_top;
}

int stairs::get_y_top(){
    return y_top;
}

int stairs::get_x_down(){
    return x_down;
}

int stairs::get_y_down(){
    return y_down;
}