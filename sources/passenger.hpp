#ifndef PASSENGER_H
#define PASSENGER_H

#include <thread>
#include "transfer_center.hpp"
#include <ncurses.h>
#include <random>
#include "target.hpp"

class transfer_center;

class passenger
{
public:
    passenger(int id, target main_target, bool top, bool tired, bool ticket, transfer_center& tc, std::atomic<bool>& end);

    int get_id();
    std::thread& get_thread();

    ~passenger();

private:
    int id;
    target main_target;
    bool top;
    bool tired;
    bool ticket;
    transfer_center& tc;
    std::atomic<bool>& end;
    std::thread passenger_thread;

    std::mt19937 rng{ std::random_device{}() };
    target next_target;

    void lifeline();
};

#endif
