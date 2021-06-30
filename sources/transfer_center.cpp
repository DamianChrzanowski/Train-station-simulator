#include "transfer_center.hpp"

#include <string>

void transfer_center::run() {
    printer.print_info("START SIMULATION");
    printer.print_station();
    //printer.print_bus(5, 2, "EAST");
    // printer.print_group(44, 6, 99);
    // printer.print_group(50, 6, 99);

    // printer.print_group(144, 55, 99);
    // printer.print_group(150, 55, 99);

    spawn_passengers_thread = std::thread(&transfer_center::spawn_passengers, this);
    spawn_west_bus_thread = std::thread(&transfer_center::spawn_west_bus, this);
    spawn_east_bus_thread = std::thread(&transfer_center::spawn_east_bus, this);
    spawn_west_subway_thread = std::thread(&transfer_center::spawn_west_subway, this);
    spawn_east_subway_thread = std::thread(&transfer_center::spawn_east_subway, this);
}

transfer_center::~transfer_center()
{
    spawn_passengers_thread.join();
    spawn_west_bus_thread.join();
}

void transfer_center::stop() {
    printer.print_info("END SIMULATION");
    end = true;
}


screen_printer& transfer_center::get_screen_printer()
{
    return printer;
}

int transfer_center::find_passenger_index(int id) 
{
    for (std::size_t i{ 0 }; i < passengers.size(); ++i)
		if (passengers[i].get()->get_id() == id)
			return i;

    return -1;
}

std::array<std::array<place, 5>, 2>& transfer_center::get_lines_tm()
{
    return lines_tm;
}

std::array<ticket_machine, 2>& transfer_center::get_ticket_machines()
{
    return ticket_machines;
}

std::array<stairs, 2>& transfer_center::get_tc_stairs()
{
    return tc_stairs;
}

std::array<bench, 8>& transfer_center::get_top_benches()
{
    return top_benches;
}

std::array<bench, 8>& transfer_center::get_down_benches()
{
    return down_benches;
}

void transfer_center::remove_passenger(int id)
{
    auto index_to_remove = find_passenger_index(id);
    if (index_to_remove != -1) {
        std::swap(passengers[index_to_remove], passengers.back());
		passengers.pop_back();
    }
}

void transfer_center::spawn_passengers() {
    // passengers.emplace_back(std::make_unique<passenger>(next_id, target::bus_west, true, false, true, *this, end));

    while (!end) {
        std::uniform_int_distribution<> wait_dis(5, 30);
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_dis(rng) * 100));
        
        // create passenger
        std::uniform_int_distribution<> dis(0, 99);
        target targets[] = {target::bus_west, target::bus_east, target::subway_west, target::subway_west};
        
        int result = dis(rng);
        target main_target = targets[result / 25];

        result = dis(rng);
        bool is_tired = result < 20;

        result = dis(rng);
        bool has_ticket = result < 10;

        // 3. arg should be true !!!
        passengers.emplace_back(std::make_unique<passenger>(next_id, main_target, true, is_tired, has_ticket, *this, end));
        ++next_id;

        // print info abut new passenger
        std::string info = "passanger number " + std::to_string(next_id) + " appeared";
        printer.print_info(&info[0]);
    }
}

void transfer_center::spawn_west_bus()
{
    std::uniform_int_distribution<> dis(30, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));
   

    while (!end) {
        bus_west = std::make_shared<bus>(37, 2, "WEST", *this, end);
        bus_west_channel.notify_all();

        dis = std::uniform_int_distribution<>(150, 190);
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));
    }
}

void transfer_center::spawn_east_bus()
{
    std::uniform_int_distribution<> dis(30, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));

    while (!end) {
        bus_east = std::make_shared<bus>(37, 57, "EAST", *this, end);
        bus_east_channel.notify_all();

        dis = std::uniform_int_distribution<>(150, 190);
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));
    }
}

void transfer_center::spawn_west_subway()
{
    std::uniform_int_distribution<> dis(30, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));

    while (!end) {
        subway_west = std::make_shared<subway>(140, 2, "WEST", *this, end);
        subway_west_channel.notify_all();

        dis = std::uniform_int_distribution<>(150, 190);
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));
    }
}

void transfer_center::spawn_east_subway()
{
    std::uniform_int_distribution<> dis(30, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));

    while (!end) {
        subway_east = std::make_shared<subway>(140, 57, "EAST", *this, end);
        subway_east_channel.notify_all();

        dis = std::uniform_int_distribution<>(150, 190);
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));
    }
}

std::shared_ptr<bus> transfer_center::get_west_bus()
{
    return bus_west;
}

std::shared_ptr<bus> transfer_center::get_east_bus()
{
    return bus_east;
}

std::shared_ptr<subway> transfer_center::get_west_subway()
{
    return subway_west;
}

std::shared_ptr<subway> transfer_center::get_east_subway()
{
    return subway_east;
}