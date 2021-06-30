#ifndef STATION_SCREEN_PRINTER_H
#define STATION_SCREEN_PRINTER_H

#include <mutex>
#include "ncurses.h"

class screen_printer {
public:
    void print_station();

    void print_group(int x, int y, int id);
    void clear_group(int x, int y);

    void print_bus(int x, int y, int number, int current, int full);
    void print_bus(int x, int y, char* line_name);
    void print_subway(int x, int y, char* line_name);
    void clear_bus(int x, int y);
    void print_info(char* info);
    
private:
    std::mutex print_mutex;
    int y{2};
};


#endif //STATION_SCREEN_PRINTER_H
