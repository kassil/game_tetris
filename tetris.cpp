#include "board.hpp"
#include "my_array.tpp"
#include "mycurses.h"
#include <array>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

enum Cmd
{
    Cmd_None,
    Cmd_Down,
    Cmd_Left,
    Cmd_Right,
    Cmd_RotL,
    Cmd_RotR,
};

enum MoveResult
{
    Blocked,
    Moved,
    Arrested,
};

struct Game
{
    // Define the board and piece matrices
    board_type board;
    piece_type piece;

    // Define the current position of the falling piece
    // i is the current y-coordinate (row) of the top-left corner of the falling piece
    // j is the current x-coordinate (column) of the top-left corner of the falling piece
    index_type piece_pos;
    size_t piece_n_moves;

    // Define the score
    int score;
};

void start_game(Game&);
void start_piece(Game&);
int move_piece(Cmd direction, board_type const& board,  piece_type& piece, index_type& pos);
void rotate_piece(piece_type& piece, Cmd direction);
void collapse_rows(Game&);

using namespace std;

// Function to initialize the game
void start_game(Game& game)
{
    // Initialize the random number generator
    srand(time(nullptr));

    // Initialize the board
    for (auto &row : game.board) {
        for (auto &cell : row) {
            cell = false;
        }
    }

    game.score = 0;

    // Generate the first piece
    start_piece(game);
}

// Function to generate a new piece
void start_piece(Game& game)
{
    // Generate a random piece
    // Set the piece position variables
    game.piece_pos =
    {
        .i = 0,
        .j = (BOARD_WIDTH - 4) / 2
    };
    game.piece_n_moves = 0;

    select_piece(game.piece);
}

// Function to move the falling piece
/*
This function takes an integer parameter direction that specifies the direction in which the falling piece should be moved. A value of 1 means move down, 2 means move left, and 3 means move right.

The function first updates the position of the falling piece according to the specified direction. It then checks for collisions by iterating over each cell in the falling piece and checking if it collides with any occupied cell on the game board. If a collision is detected, the function locks the falling piece in place by copying its cells to the corresponding cells on the game board and generates a new piece. Otherwise, the function simply returns.
*/
int move_piece(Cmd direction, board_type const& board, piece_type& piece, index_type& pos)
{
    // Move the piece in the specified direction
    if (direction == Cmd_Down)
    {
        // Move down
        if (collide(board, piece, pos.i + 1, pos.j))
        {
            return Arrested;
        }
        else
        {
            pos.i++;
            return Moved;
        }
    }
    else if (direction == Cmd_Left)
    {
        // Move left
        if (!collide(board, piece, pos.i, pos.j - 1))
        {
            pos.j--;
            return Moved;
        }
        else
        {
            return Blocked;
        }
    }
    else if (direction == Cmd_Right)
    {
        // Move right
        if (!collide(board, piece, pos.i, pos.j + 1))
        {
            pos.j++;
            return Moved;
        }
        else
        {
            return Blocked;
        }
    }
    else if (direction == Cmd_RotL || direction == Cmd_RotR)
    {
        piece_type rot_piece = piece;
        rotate_piece(rot_piece, direction);
        if (collide(board, rot_piece, pos.i, pos.j))
        {
            return Blocked;
        }
        else
        {
            piece = rot_piece;
            return Moved;
        }
    }
    else
    {
        wprintw(mycurses->log_win, "%s(%d) unknown\n", __func__, direction);
        return Blocked;
    }
}

// Rotate the falling piece in the specified direction
void rotate_piece(piece_type& piece, Cmd direction)
{
    size_t n_iter = (Cmd_RotL == direction) ? 3 : 1;
    for(size_t i = 0; i < n_iter; ++i)
        rotate_array(piece);
}

// Function to check for completed lines
/*
This function checks each row of the game board from bottom to top to see if it is complete. A row is considered complete if all its cells are occupied. If a complete row is found, it is removed by setting all its cells to zero. Then, all the rows above it are shifted down by one cell to fill the gap. The function then checks the same row again, since a shifted row may also be complete. This process continues until all complete rows have been removed.

The function also increases the score by one for each complete row that is removed.
*/
void collapse_rows(Game& game)
{
    auto& board = game.board;
    // Check each line for completeness
    // If a line is complete, remove it and shift all the lines above it down
    // Update the score
    for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
        bool line_complete = true;
        for (size_t j = 0; j < BOARD_WIDTH; j++) {
            if (!board[i][j]) {
                line_complete = false;
                break;
            }
        }
        if (line_complete) {
            // Remove the completed line
            std::fill(board[i].begin(), board[i].end(), 0);
            // Shift all the lines above it down
            for (int k = i - 1; k >= 0; k--) {
                std::copy(board[k].begin(), board[k].end(), board[k+1].begin());
            }
            i++; // Check the same line again
            game.score++; // Increase the score
            mycurses->updateScore(game.score);
            print_board(board, game.piece, game.piece_pos);
            wprintw(mycurses->log_win, "Score %d\n", game.score);
            wrefresh(mycurses->log_win);
            usleep(1000 * 1000);
        }
    }
}

int main()
{
    MyCurses curses;
    Game game;
    start_game(game);

    while (true)
    {
        // Print the game board
        print_board(game.board, game.piece, game.piece_pos);

        int ch = wgetch(mycurses->log_win);
        Cmd cmd;
        switch(ch)
        {
        case KEY_RESIZE:
            mycurses->resize();
            continue;
        case 'a':
        case KEY_LEFT:
            // Handle left arrow key
            cmd = Cmd_Left;
            break;
        case 'd':
        case KEY_RIGHT:
            // Handle right arrow key
            cmd = Cmd_Right;
            break;
        case 'q':
            cmd = Cmd_RotL;
            break;
        case 'e':
            cmd = Cmd_RotR;
            break;
        // case KEY_UP:
        //     // Handle up arrow key
        //     break;
        case 's':
        case KEY_DOWN:
            // Handle down arrow key
            cmd = Cmd_Down;
            break;
        case 27: //ESC
            if (dialog_quit())
                return 0;
            else
                continue;
            break;
        default:
            wprintw(mycurses->log_win, "Unknown key 0%o 0x%X %d\n", ch, ch, ch);
            wrefresh(mycurses->log_win);
            continue;
        }

        // Move the falling piece down
        int moved = move_piece(cmd, game.board, game.piece, game.piece_pos);
        if (moved == Blocked)
        {
            wprintw(mycurses->log_win, "Lateral blocked\n");
        }
        else if (moved == Moved)
        {
            wprintw(mycurses->log_win, "Moved\n");
            if (cmd == Cmd_Down)
                game.piece_n_moves++;
        }
        else if (moved == Arrested)
        {
            // Fall arrested
            wprintw(mycurses->log_win, "Fall arrested\n");
            anchor(game.board, game.piece, game.piece_pos);
            if (game.piece_n_moves == 0)
            {
                wprintw(mycurses->log_win, "Game over -- Score %d\n", game.score);
                break;
            }
            // Generate a new piece
            start_piece(game);
        }
        else
        {
            wprintw(mycurses->log_win, "Move result? %d\n", moved);
        }
        
        // box(log_win, 0, 0);
        wrefresh(mycurses->log_win);

        // Check for completed lines
        int old_score = game.score;
        collapse_rows(game);
        if (old_score != game.score)
        {
            // // Print the game board
            // print_board(game.board, game.piece, game.piece_pos);
            // Add a line of text to the log window
            wprintw(mycurses->log_win, "Score %d --> %d\n", old_score, game.score);
            wrefresh(mycurses->log_win);
        }
    }
    // Clean up and exit
    return 0;
}
