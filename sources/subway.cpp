#include "subway.hpp"

subway::subway(int x, int y, std::string line_name, transfer_center& tc, std::atomic<bool>& end) :
        x(x), y(y), line_name(line_name), tc(tc), end(end), subway_thread(&subway::lifeline, this) {}

std::array<door, 2>& subway::get_doors()
{
    return doors;
}

void subway::get_in(std::vector<std::unique_ptr<passenger>>& passengers, int index)
{
    std::swap(passengers[index], passengers.back());
    subway_passengers.emplace_back(std::move(passengers.back()));
}

void subway::lifeline() 
{
    std::string info;
    screen_printer& printer = tc.get_screen_printer();

    // arrival stage

    printer.print_subway(x, y, &line_name[0]);

    info = line_name + " arrived";
    printer.print_info(&info[0]);

    std::uniform_int_distribution<> dis(2, 4);
    std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));

    // opening doors stage

    st = stage::opening_doors;

    for (auto& d : doors)
        d.set_open(true);

    info = line_name + " opened doors";
    printer.print_info(&info[0]);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // release passengers stage

    st = stage::release_passengers;

    info = line_name + " releases passengers";
    printer.print_info(&info[0]);

    dis = std::uniform_int_distribution<>(20, 40);
    std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));

    // admitting passengers stage

    st = stage::admitting_passengers;

    info = line_name + " lets passengers in";
    printer.print_info(&info[0]);

    dis = std::uniform_int_distribution<>(20, 40);
    std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));
    
    // closing doors stage

    st = stage::closing_doors;

    for (auto& d : doors)
        d.set_open(false);

    info = line_name + " closed doors";
    printer.print_info(&info[0]);

    std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));

    // closing doors stage

    st = stage::departure;

    info = line_name + " departs";
    printer.print_info(&info[0]);

    printer.clear_bus(x, y);
}

stage subway::get_stage()
{
    return st;
}

subway::~subway()
{
    subway_thread.join();
}