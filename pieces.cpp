#include "mycurses.h"
#include "my_array.tpp"
#include <iostream>
#include <stdlib.h>  //rand

using namespace std;

int rand_a(const int N)
{
    return rand() / (RAND_MAX / N);
}

void select_piece(piece_type& piece)
{
    constexpr piece_type I_piece = {{
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
    }};
    constexpr piece_type J_piece = {{
        {0, 0, 0, 0},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 1, 1},
    }};
    constexpr piece_type L_piece = {{
        {0, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 1, 0, 0},
    }};
    constexpr piece_type O_piece = {{
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    }};
    constexpr piece_type S_piece = {{
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    }};
    constexpr piece_type T_piece = {{
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    }};
    constexpr piece_type Z_piece = {{
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    }};
    // Choose a piece
    int r = rand_a(7);

    if (r == 0)
        piece = I_piece;
    else if (r == 1)
        piece = J_piece;
    else if (r == 2)
        piece = L_piece;
    else if (r == 3)
        piece = O_piece;
    else if (r == 4)
        piece = S_piece;
    else if (r == 5)
        piece = T_piece;
    else if (r == 6)
        piece = Z_piece;
    else
    {
        wprintw(mycurses->log_win, "rand out of range %d\n", r);
        wrefresh(mycurses->log_win);
        //cerr << "rand out of range " << r << endl;
    }

    // Mirror image
    r = rand_a(2);
    if (r)
        transpose_array(piece);
}
