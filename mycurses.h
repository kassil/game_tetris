#pragma once
#include <ncurses.h>

class MyCurses
{
public:
    MyCurses();
    ~MyCurses();

    WINDOW* log_win;
    WINDOW* main_win;
    // WINDOW* title_win;

    int max_y, max_x;
    int log_height;
    int main_height;
    int main_width;
};

extern MyCurses* mycurses;
