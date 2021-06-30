#include "ticket_machine.hpp"

ticket_machine::ticket_machine(int x, int y): x(x), y(y) {}

void ticket_machine::request(int other_owner) {
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

int ticket_machine::get_owner() {
    return owner;
}

void ticket_machine::release() {
    owner = -1;
    free = true;
    channel.notify_all();
}

bool ticket_machine::is_free() {
    return free;
}

std::mutex& ticket_machine::get_mutex() {
    return mutex;
}

int ticket_machine::get_x(){
    return x;
}

int ticket_machine::get_y(){
    return y;
}