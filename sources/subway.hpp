#ifndef SUBWAY_H
#define SUBWAY_H

#include <array>
#include <random>
#include <vector>
#include <atomic>
#include <memory>
#include <ncurses.h>
#include <iostream>
#include <thread>
#include "door.hpp"
#include "seat.hpp"
#include "passenger.hpp"
#include "stage.hpp"

class transfer_center;
class passenger;

class subway {
public:
    subway(int x, int y, std::string line_name, transfer_center& tc, std::atomic<bool>& end);

    std::array<door, 2>& get_doors();
    void get_in(std::vector<std::unique_ptr<passenger>>& passengers, int index);
    stage get_stage();
    ~subway();

private:
    int x;
    int y;
    std::string line_name;
    transfer_center& tc;

    std::atomic<bool>& end;

    std::thread subway_thread;

    stage st = stage::arrival;

    std::vector<std::unique_ptr<passenger>> subway_passengers;

    std::array<door, 2> doors {
        door(10, 10),
        door(10, 10)
    };

    std::mt19937 rng{ std::random_device{}() };

    void lifeline();
};

#endif