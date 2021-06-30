#ifndef TRANSFER_CENTER_H
#define TRANSFER_CENTER_H

#include <random>
#include <atomic>
#include <memory>

#include <ncurses.h>

#include "screen_printer.hpp"
#include "passenger.hpp"
#include "bus.hpp"
#include "subway.hpp"
#include "target.hpp"
#include "place.hpp"
#include "stairs.hpp"
#include "ticket_machine.hpp"
#include "bench.hpp"

class passenger;
class bus;
class subway;

class transfer_center {
public:
    void run();
    void stop();

    screen_printer& get_screen_printer();
    int find_passenger_index(int id);
    std::array<std::array<place, 5>, 2>& get_lines_tm();
    std::array<ticket_machine, 2>& get_ticket_machines();
    std::array<stairs, 2>& get_tc_stairs();
    std::array<bench, 8>& get_top_benches();
    std::array<bench, 8>& get_down_benches();
    void remove_passenger(int id);

    sync_channel bus_west_channel;
    sync_channel bus_east_channel;
    sync_channel subway_west_channel;
    sync_channel subway_east_channel;

    std::shared_ptr<bus> get_west_bus();
    std::shared_ptr<bus> get_east_bus();
    std::shared_ptr<subway> get_west_subway();
    std::shared_ptr<subway> get_east_subway();

    ~transfer_center();

private:
    // for driwing map of transport center
    screen_printer printer;
    
    // list of passengers
    std::vector<std::unique_ptr<passenger>> passengers;
    int next_id{0};

    // transports
    std::shared_ptr<bus> bus_west;
    std::shared_ptr<bus> bus_east;
    std::shared_ptr<subway> subway_west;
    std::shared_ptr<subway> subway_east;

    std::atomic<bool> end = false;
    std::mt19937 rng{ std::random_device{}() };

    // threads executed by the class
    std::thread spawn_passengers_thread;
    std::thread spawn_west_bus_thread;
    std::thread spawn_east_bus_thread;
    std::thread spawn_west_subway_thread;
    std::thread spawn_east_subway_thread;

    void spawn_passengers();
    void spawn_west_bus();
    void spawn_east_bus();
    void spawn_west_subway();
    void spawn_east_subway();

    // ticket machines and lines to them
    std::array<ticket_machine, 2> ticket_machines {
        ticket_machine(11, 18),
        ticket_machine(11, 43)
    };

    std::array<std::array<place, 5>, 2> lines_tm { { 
            {   
                place(1, 17, 18),
                place(2, 23, 18),
                place(3, 29, 18),
                place(4, 35, 18),
                place(5, 41, 18)      
            },

            {   
                place(1, 17, 43),
                place(2, 23, 43),
                place(3, 29, 43),
                place(4, 35, 43),
                place(5, 41, 43)      
            }
    } };

    int stairs_x_1 = 35;
    int stairs_y_1 = 23;
    int stairs_x_2 = 135;
    int stairs_y_2 = 23;

    int stairs_x_3 = 35;
    int stairs_y_3 = 37;
    int stairs_x_4 = 135;
    int stairs_y_4 = 37;

    // stairs for moving between levels
    std::array<stairs, 2> tc_stairs {
        // top: x, y, down: x, y
        stairs(30, 24, 166, 24),
        stairs(30, 38, 166, 38)
    };

    // benches for tired passengers
    std::array<bench, 8> top_benches {
        bench(16, 8),
        bench(36, 8),
        bench(56, 8),
        bench(76, 8),
        bench(16, 53),
        bench(36, 53),
        bench(56, 53),
        bench(76, 53)
    };

    std::array<bench, 8> down_benches {
        bench(121, 8),
        bench(141, 8),
        bench(161, 8),
        bench(181, 8),
        bench(121, 53),
        bench(141, 53),
        bench(161, 53),
        bench(181, 53)
    };

};

#endif
