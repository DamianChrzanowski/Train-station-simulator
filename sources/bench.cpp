#include "bench.hpp"

bench::bench(int x, int y): x(x), y(y) {}

void bench::request(int other_owner) {
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

int bench::get_owner() {
    return owner;
}

void bench::release() {
    owner = -1;
    free = true;
    channel.notify_all();
}

bool bench::is_free() {
    return free;
}

std::mutex& bench::get_mutex() {
    return mutex;
}

int bench::get_x(){
    return x;
}

int bench::get_y(){
    return y;
}