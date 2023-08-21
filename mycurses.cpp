#include "mycurses.h"
#include "my_array.tpp"

MyCurses* mycurses;

MyCurses::MyCurses()
{
    // Initialize ncurses
    // initialize the screen, get the size of the terminal, and create two
    // windows: a log window at the bottom and a main window at the top. The
    // height of the log window is set to one fourth of the height of the
    // terminal, and the height of the main window is set to the remaining
    // three fourths of the height of the terminal.
    initscr();
    cbreak();  // no line buffering
    noecho();
    keypad(stdscr, true);  // Use arrow keys
    curs_set(0);  // Hide the cursor

    // Get the size of the terminal
    getmaxyx(stdscr, max_y, max_x);

    // Define the size of the main window
    main_height = BOARD_HEIGHT + 3;

    // Define the size of the log window (height scales with the terminal)
    log_height = std::max(max_y - main_height, 3);

    // We also add a box around each window using the box function, and
    // refresh the windows using the wrefresh function. Finally, we wait
    // for a key press using the getch function, and clean up and exit using
    // the delwin and endwin functions.

    // Create the main window
    main_width = 3 + 2 * BOARD_WIDTH;
    main_win = newwin(main_height, main_width, 0, 0);
    keypad(main_win, true);  // Use arrow keys
    box(main_win, 0, 0);

    // Create the log window
    log_win = newwin(log_height, max_x, main_height, 0);
    keypad(log_win, true);  // Use arrow keys
    scrollok(log_win, TRUE);
    wprintw(log_win, "New game\n");
    wrefresh(log_win);

    mycurses = this;
}

MyCurses::~MyCurses()
{
    mycurses = nullptr;
    // Reset the terminal
    delwin(log_win);
    delwin(main_win);
    endwin();
}

void MyCurses::resize()
{
    // Get the size of the terminal
    getmaxyx(stdscr, max_y, max_x);
    // Define the size of the log window (height scales with the terminal)
    log_height = std::max(max_y - main_height, 3);
    wresize(log_win, log_height, max_x);
    wprintw(log_win, "Resize %dx%d %dx%d\n",max_y,max_x,log_height,max_x);
    wrefresh(log_win);
}

void MyCurses::updateScore(int score)
{
    mvwprintw(main_win, 1, 1, "Score %d", score);
    //wrefresh(main_win);
}

// void calc_size()
// {
//     // Get the size of the terminal
//     getmaxyx(stdscr, max_y, max_x);

//     // Define the size of the main window
//     main_height = BOARD_HEIGHT + 3;

//     // Define the size of the log window (height scales with the terminal)
//     log_height = std::max(max_y - main_height, 3);

//     // We also add a box around each window using the box function, and
//     // refresh the windows using the wrefresh function. Finally, we wait
//     // for a key press using the getch function, and clean up and exit using
//     // the delwin and endwin functions.

//     // Create the main window
//     main_width = 3 + 2 * BOARD_WIDTH;

// }

bool dialog_quit()
{
    wprintw(mycurses->log_win, "Quit? (Y/N)\n");
    wrefresh(mycurses->log_win);
    flushinp();  // Discard type-ahead
    while (true)
    {
        int ch = wgetch(mycurses->log_win);
        switch(ch)
        {
        case 'y': case 'Y':
            return true;
        case 'n': case 'N': case 27:
            wprintw(mycurses->log_win, "Continuing\n");
            wrefresh(mycurses->log_win);
            return false;
        }
    }
}
