#include "mycurses.h"
#include "my_array.tpp"

MyCurses::MyCurses()
{
    // Initialize ncurses
    // initialize the screen, get the size of the terminal, and create two windows: a log window at the bottom and a main window at the top. The height of the log window is set to one fourth of the height of the terminal, and the height of the main window is set to the remaining three fourths of the height of the terminal.
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);

    // Get the size of the terminal
    getmaxyx(stdscr, max_y, max_x);

    // Define the size of the main window
    main_height = BOARD_HEIGHT + 3;

    // Define the size of the log window (height scales with the terminal)
    log_height = std::max(max_y - main_height, 3);

    //We also add a box around each window using the box function, and refresh the windows using the wrefresh function. Finally, we wait for a key press using the getch function, and clean up and exit using the delwin and endwin functions.

    // Create the main window
    main_width = 3 + 2 * BOARD_WIDTH;
    main_win = newwin(main_height, main_width, 0, 0);
    box(main_win, 0, 0);
    wrefresh(main_win);

    // Create the log window
    log_win = newwin(log_height, max_x, main_height, 0);
    //box(log_win, 0, 0);
    //wmove(log_win, 1, 1);
    scrollok(log_win, TRUE);
    wrefresh(log_win);
}

MyCurses::~MyCurses()
{
    // Reset the terminal
    delwin(log_win);
    delwin(main_win);
    endwin();
}
