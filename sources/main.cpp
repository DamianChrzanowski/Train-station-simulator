#include <iostream>
#include <ncurses.h>
#include "transfer_center.hpp"

void run() {
    std::cout << "\e[8;65;238t";
    initscr();
    refresh();

    if (!has_colors())
        printw("colors are not supported");

    start_color();
    resizeterm(64,237);

    transfer_center tc;

    tc.run();

    int ch = 0;

    do {
        ch = getch();
    } while(ch != 27);

    tc.stop();
}

int main() {
    run();

    return 0;
}




