#include <iostream>
#include "screen_printer.hpp"

//0000	(0)	Black
//0001	(1)	Blue
//0010	(2)	Green
//0011	(3)	Cyan
//0100	(4)	Red
//0101	(5)	Magenta
//0110	(6)	Yellow
//0111	(7)	White
//1000	(8)	Bright Black
//1001	(9)	Bright Blue
//1010	(10)	Bright Green
//1011	(11)	Bright Cyan
//1100	(12)	Bright Red
//1101	(13)	Bright Magenta
//1110	(14)	Bright Yellow
//1111	(15)	Bright White

void screen_printer::print_station()
{
    init_pair(1, 0, 15);
    attron(COLOR_PAIR(1));
    int num{0};
    for (int i {0}; i <= 237; ++i) {
        mvprintw(0, i, "%d",num);
        num++;
        if (num == 10)
            num =0;

        mvprintw(62, i, " ");

        // if (i >= 100 && i <= 200)
        //     mvprintw(31, i, " ");
    }

    for (int i{0}; i<= 62; ++i) {

        if (i <= 27 || i >= 35)
            mvprintw(i, 0, "  ");

        mvprintw(i, 99, "%2d",i);
        mvprintw(i, 200, "  ");
        mvprintw(i, 235, "  ");
    }
    attroff(COLOR_PAIR(1));

    // tory
    init_pair(2, 3, 3);
    attron(COLOR_PAIR(2));
    for (int i{2}; i<= 98; ++i) {
        mvprintw(1, i, " ");
        mvprintw(2, i, " ");
        mvprintw(3, i, " ");
        mvprintw(4, i, " ");
        mvprintw(5, i, " ");

        mvprintw(57, i, " ");
        mvprintw(58, i, " ");
        mvprintw(59, i, " ");
        mvprintw(60, i, " ");
        mvprintw(61, i, " ");

    }

    for (int i{101}; i<= 199; ++i) {
        mvprintw(1, i, " ");
        mvprintw(2, i, " ");
        mvprintw(3, i, " ");
        mvprintw(4, i, " ");
        mvprintw(5, i, " ");

        // mvprintw(26, i, " ");
        // mvprintw(27, i, " ");
        // mvprintw(28, i, " ");
        // mvprintw(29, i, " ");
        // mvprintw(30, i, " ");

        // mvprintw(32, i, " ");
        // mvprintw(33, i, " ");
        // mvprintw(34, i, " ");
        // mvprintw(35, i, " ");
        // mvprintw(36, i, " ");

        mvprintw(57, i, " ");
        mvprintw(58, i, " ");
        mvprintw(59, i, " ");
        mvprintw(60, i, " ");
        mvprintw(61, i, " ");
    }
    attroff(COLOR_PAIR(2));

    init_pair(3,COLOR_WHITE, COLOR_BLUE);
    attron(COLOR_PAIR(3));

    for (int i = 0; i<= 3;i++){
        mvprintw(i+17, 2, "         ");
    }
    mvprintw(17, 3, "Tickets");


    for (int i = 0; i<= 3;i++){
        mvprintw(i+42, 2, "         ");
    }
    mvprintw(42, 3, "Tickets");

    attroff(COLOR_PAIR(3));

    init_pair(7,COLOR_WHITE, COLOR_GREEN);
    attron(COLOR_PAIR(7));

    int stairs_x_1 = 35;
    int stairs_y_1 = 23;
    int stairs_x_2 = 135;
    int stairs_y_2 = 23;

    int stairs_x_3 = 35;
    int stairs_y_3 = 36;
    int stairs_x_4 = 135;
    int stairs_y_4 = 36;

    for (int i = 0; i < 4;i++){
        mvprintw(i + stairs_y_1, stairs_x_1, "                              ");
        mvprintw(i + stairs_y_2, stairs_x_2, "                              ");
        mvprintw(i + stairs_y_3, stairs_x_3, "                              ");
        mvprintw(i + stairs_y_4, stairs_x_4, "                              ");

    }

    attroff(COLOR_PAIR(7));

    // benches
    init_pair(9, COLOR_WHITE, COLOR_MAGENTA);
    attron(COLOR_PAIR(9));

    int bench_x_1 = 15;
    int bench_y_1 = 10;
    int bench_x_2 = 120;
    int bench_y_2 = 10;

    int bench_x_3 = 15;
    int bench_y_3 = 52;
    int bench_x_4 = 120;
    int bench_y_4 = 52;

    for (int i = 0; i < 4;i++){

        mvprintw(bench_y_1, bench_x_1 + (i * 20), "      "  );
        mvprintw(bench_y_2, bench_x_2 + (i * 20), "      ");
        mvprintw(bench_y_3, bench_x_3 + (i * 20), "      ");
        mvprintw(bench_y_4, bench_x_4 + (i * 20), "      ");

    }

    attroff(COLOR_PAIR(9));
}

void screen_printer::print_group(int x,int y,int id)
{
    std::lock_guard<std::mutex> print_lock(print_mutex);
    init_pair(4,COLOR_BLACK, COLOR_YELLOW);
    attron(COLOR_PAIR(4));
    mvprintw(y,x,"    ");
    mvprintw(y+1,x,"  %2d",id);
    attroff(COLOR_PAIR(4));
    refresh();
}

void screen_printer::clear_group(int x, int y)
{
    std::lock_guard<std::mutex> print_lock(print_mutex);
    init_pair(5,COLOR_BLACK, COLOR_BLACK);
    attron(COLOR_PAIR(5));
    mvprintw(y,x,"XXXX");
    mvprintw(y+1,x,"XXXX");
    attroff(COLOR_PAIR(5));
    refresh();
}

void screen_printer::print_bus(int x, int y, int number, int current, int full)
{
    std::lock_guard<std::mutex> print_lock(print_mutex);
    init_pair(6,COLOR_WHITE, COLOR_RED);
    attron(COLOR_PAIR(6));
    mvprintw(y,x,"         %2d          ",number);
    mvprintw(y+1,x,"                     ");
    mvprintw(y+2,x,"       %2d / %2d       ",full);
    mvprintw(y+3,x,"                     ",current);

    attroff(COLOR_PAIR(6));
    refresh();
}

void screen_printer::print_info(char* info)
{
    std::lock_guard<std::mutex> print_lock(print_mutex);
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(8));
    mvprintw(y, 205, "                                 ");
    mvprintw(y, 205, info);
    y = (y + 1) % 61;
    attroff(COLOR_PAIR(8));
    refresh();
}

void screen_printer::print_bus(int x, int y, char* line_name)
{
    std::lock_guard<std::mutex> print_lock(print_mutex);
    init_pair(6,COLOR_WHITE, COLOR_RED);
    attron(COLOR_PAIR(6));
    mvprintw(y,     x,"         %s        ", line_name);
    mvprintw(y + 1, x,"                     ");
    mvprintw(y + 2, x,"                     ");
    mvprintw(y + 3, x,"                     ");

    attroff(COLOR_PAIR(6));
    refresh();
}

void screen_printer::print_subway(int x, int y, char* line_name)
{
    std::lock_guard<std::mutex> print_lock(print_mutex);
    init_pair(11,COLOR_WHITE, 9);
    attron(COLOR_PAIR(11));
    mvprintw(y,     x,"         %s        ", line_name);
    mvprintw(y + 1, x,"                     ");
    mvprintw(y + 2, x,"                     ");
    mvprintw(y + 3, x,"                     ");

    attroff(COLOR_PAIR(11));
    refresh();
}

void screen_printer::clear_bus(int x, int y)
{
    std::lock_guard<std::mutex> print_lock(print_mutex);
    init_pair(2, 3, 3);
    attron(COLOR_PAIR(2));
    mvprintw(y,     x,"                     ");
    mvprintw(y + 1, x,"                     ");
    mvprintw(y + 2, x,"                     ");
    mvprintw(y + 3, x,"                     ");

    attroff(COLOR_PAIR(2));
    refresh();
}