#pragma once
#include <ncurses.h>

class MyCurses
{
public:
    MyCurses();
    ~MyCurses();

    void resize();
    void updateScore(int score);

    WINDOW* log_win;
    WINDOW* main_win;
    // WINDOW* title_win;

    int max_y, max_x;
    int log_height;
    int main_height;
    int main_width;
};

bool dialog_quit();

extern MyCurses* mycurses;
