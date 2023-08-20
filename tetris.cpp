#include <iostream>
#include <cstdlib>
#include <ctime>

enum Cmd {
    Cmd_None,
    Cmd_Down,
    Cmd_Left,
    Cmd_Right,
    Cmd_RotL,
    Cmd_RotR,
};

void init_game();
void generate_piece();
void move_piece(Cmd direction);
void rotate_piece(int direction);
void check_lines();
void print_board();

#include <array>

using namespace std;

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;

// Define the board and piece matrices
std::array<std::array<int, BOARD_WIDTH>, BOARD_HEIGHT> board;
std::array<std::array<int, 4>, 4> piece;

// Define the current position of the falling piece
// piece_y is the current y-coordinate (row) of the top-left corner of the falling piece
int piece_x, piece_y;

// Define the score
int score = 0;

// Function to initialize the game
void init_game() {
    // Initialize the random number generator
    srand(time(nullptr));

    // Initialize the board
    for (auto &row : board) {
        for (auto &cell : row) {
            cell = 0;
        }
    }

    // Generate the first piece
    generate_piece();
}

// Function to generate a new piece
void generate_piece()
{
    // Generate a random piece
    // Set the piece_x and piece_y variables
    piece_x = (BOARD_WIDTH - 4) / 2;
    piece_y = 3;

    constexpr std::array<std::array<int, 4>, 4> L_piece = {{
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 1, 1},
    }};

    piece = L_piece;
}

// Function to move the falling piece
/*
This function takes an integer parameter direction that specifies the direction in which the falling piece should be moved. A value of 1 means move down, 2 means move left, and 3 means move right.

The function first updates the position of the falling piece according to the specified direction. It then checks for collisions by iterating over each cell in the falling piece and checking if it collides with any occupied cell on the game board. If a collision is detected, the function locks the falling piece in place by copying its cells to the corresponding cells on the game board and generates a new piece. Otherwise, the function simply returns.
*/
void move_piece(Cmd direction)
{
    // Move the piece in the specified direction
    if (direction == Cmd_Down)
    { // Move down
        piece_y++;
    }
    else if (direction == Cmd_Left)
    { // Move left
        piece_x--;
    }
    else if (direction == Cmd_Right)
    { // Move right
        piece_x++;
    }

    // Check for collisions
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piece[i][j] != 0) {
                int x = piece_x + j;
                int y = piece_y + i;
                if (x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT || board[y][x] != 0) {
                    // Collision detected, lock the piece in place
                    cerr << "Collide piece(" << j << ',' << i << ") board(" << y << ',' << x << ")" << endl;
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            if (piece[i][j] != 0) {
                                board[piece_y + i][piece_x + j] = piece[i][j];
                            }
                        }
                    }
                    // Generate a new piece
                    generate_piece();
                    return;
                }
            }
        }
    }
}

// Function to rotate the falling piece
void rotate_piece(int direction) {
    // Rotate the piece in the specified direction
    // Check for collisions
    // Lock the piece in place if there is a collision
    // Generate a new piece
    std::array<std::array<int, 4>, 4> old_piece;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            piece[i][4-j] = old_piece[i][j];
        }
    }    
}

// Function to check for completed lines
/*
This function checks each row of the game board from bottom to top to see if it is complete. A row is considered complete if all its cells are occupied. If a complete row is found, it is removed by setting all its cells to zero. Then, all the rows above it are shifted down by one cell to fill the gap. The function then checks the same row again, since a shifted row may also be complete. This process continues until all complete rows have been removed.

The function also increases the score by one for each complete row that is removed.
*/
void check_lines() {
    // Check each line for completeness
    // If a line is complete, remove it and shift all the lines above it down
    // Update the score

    for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
        bool line_complete = true;
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j] == 0) {
                line_complete = false;
                break;
            }
        }
        if (line_complete) {
            // Remove the completed line
            for (int j = 0; j < BOARD_WIDTH; j++) {
                board[i][j] = 0;
            }
            // Shift all the lines above it down
            for (int k = i - 1; k >= 0; k--) {
                for (int j = 0; j < BOARD_WIDTH; j++) {
                    board[k + 1][j] = board[k][j];
                }
            }
            i++; // Check the same line again
            score++; // Increase the score
        }
    }
}

// Function to print the game board
void print_board() {
    cout << endl << "piece(" << piece_y << ','<<piece_x<<")\n";
    for (int i = 0; i < BOARD_HEIGHT; i++) {

        for (int j = 0; j < BOARD_WIDTH; j++) {

            bool occupied = board[i][j];
            if ((i <= piece_y && piece_y < (i + 4)) &&
                (j <= piece_x && piece_x < (j + 4)))
            {
                occupied |= piece[piece_y - i][piece_x - j];
            }
            cout << (occupied ? 'o' : '.') << ' ';
        }
        cout << endl;
    }
}

#include <unistd.h>  //sleep

int main() {
    init_game();
    Cmd cmd = Cmd_Down;
    while (true) {
        // Print the game board
        print_board();

        // Move the falling piece down
        move_piece(cmd);

        // Check for completed lines
        check_lines();

        // Generate a new piece if necessary
        if (piece_y == 0) {
            generate_piece();
        }

        char ch;
        //sleep(1);
        cin >> ch;
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
            case 's':
                cmd = Cmd_Down;
                break;
            }
        }
    }
    return 0;
}