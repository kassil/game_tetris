#include "board.hpp"
#include "my_array.tpp"
#include "tetris.h"
#include <array>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

enum Cmd {
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

void select_piece(piece_type& piece);

void init_game();
void start_piece();
int move_piece(Cmd direction, piece_type& piece, index_type& pos);
void rotate_piece(piece_type& piece, Cmd direction);
void check_lines();

using namespace std;

// Define the board and piece matrices
board_type board;
piece_type piece;

// Define the current position of the falling piece
// i is the current y-coordinate (row) of the top-left corner of the falling piece
// j is the current x-coordinate (column) of the top-left corner of the falling piece
index_type piece_pos;
size_t piece_n_moves;

// Define the score
int score = 0;

// Function to initialize the game
void init_game() {
    // Initialize the random number generator
    srand(time(nullptr));

    // Initialize the board
    for (auto &row : board) {
        for (auto &cell : row) {
            cell = false;
        }
    }

    // Generate the first piece
    start_piece();
}

// Function to generate a new piece
void start_piece()
{
    // Generate a random piece
    // Set the piece position variables
    piece_pos =
    {
        .i = 0,
        .j = (BOARD_WIDTH - 4) / 2
    };
    piece_n_moves = 0;

    select_piece(piece);
}

// Function to move the falling piece
/*
This function takes an integer parameter direction that specifies the direction in which the falling piece should be moved. A value of 1 means move down, 2 means move left, and 3 means move right.

The function first updates the position of the falling piece according to the specified direction. It then checks for collisions by iterating over each cell in the falling piece and checking if it collides with any occupied cell on the game board. If a collision is detected, the function locks the falling piece in place by copying its cells to the corresponding cells on the game board and generates a new piece. Otherwise, the function simply returns.
*/
int move_piece(Cmd direction, piece_type& piece, index_type& pos)
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
        fprintf(stderr,"%s(%d) unknown\n", __func__, direction);
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
void check_lines()
{
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
                // for (size_t j = 0; j < BOARD_WIDTH; j++) {
                //     board[k + 1][j] = board[k][j];
                // }
            }
            i++; // Check the same line again
            score++; // Increase the score
            print_board(board, piece, piece_pos);
            printf("Score %d\n", score);
            usleep(500 * 1000);
        }
    }
}

// Function to print a piece
template<typename T>
void print_table(T const& table)
{
    cout << __func__ << endl;
    for (typename T::size_type i = 0; i < table.size(); i++) {

        for (typename T::size_type j = 0; j < table[i].size(); j++) {

            bool occupied = table[i][j];
            cout << (occupied ? 'o' : '.') << ' ';
        }
        cout << endl;
    }
}

int main()
{
    init_game();
    print_table(piece);
    while (true)
    {
        // Print the game board
        print_board(board, piece, piece_pos);
        char ch;
        cin >> ch;
        Cmd cmd;
        if (cin)
        {
            switch(ch)
            {
            case 'a':
                cmd = Cmd_Left;
                break;
            case 'd':
                cmd = Cmd_Right;
                break;
            case 'q':
                cmd = Cmd_RotL;
                break;
            case 'e':
                cmd = Cmd_RotR;
                break;
            case 's':
            default:
                cmd = Cmd_Down;
                break;
            }
        }
        else
        {
            cmd = Cmd_Down;
        }

        // Move the falling piece down
        int moved = move_piece(cmd, piece, piece_pos);
        if (moved == Blocked)
        {
            cout << "Lateral blocked\n";
        }
        else if (moved == Moved)
        {
            cout << "Moved\n";
            if (cmd == Cmd_Down)
                piece_n_moves++;
        }
        else if (moved == Arrested)
        {
            // Fall arrested
            cout << "Fall arrested\n";
            anchor(board, piece, piece_pos);
            if (piece_n_moves == 0)
            {
                cout << "Game over -- Score " << score << endl;
                return 0;
            }
            // Generate a new piece
            start_piece();
        }
        else
        {
            cerr << "Move result? " << moved << endl;
        }

        // Check for completed lines
        int old_score = score;
        check_lines();
        if (old_score != score)
        {
            // Print the game board
            print_board(board, piece, piece_pos);
            printf("Score %d --> %d\n", old_score, score);
        }
    }
    return 0;
}
