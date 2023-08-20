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
int move_piece(Cmd direction);
bool collide(size_t piece_y, int piece_x);
void anchor();
void rotate_piece(int direction);
void check_lines();
void print_board();

#include <array>

using namespace std;

constexpr size_t BOARD_WIDTH = 12;
constexpr size_t BOARD_HEIGHT = 12;

// Define the board and piece matrices
std::array<std::array<int, BOARD_WIDTH>, BOARD_HEIGHT> board;
std::array<std::array<int, 4>, 4> piece;

// Define the current position of the falling piece
// piece_y is the current y-coordinate (row) of the top-left corner of the falling piece
int piece_x;
size_t piece_y;
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
    piece_x = -1; //(BOARD_WIDTH - 4) / 2;
    piece_y = 0;
    piece_n_moves = 0;

    constexpr std::array<std::array<int, 4>, 4> L_piece = {{
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 1, 1},
    }};

    constexpr std::array<std::array<int, 4>, 4> I_piece = {{
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 1},
    }};

    piece = L_piece;
}

// Function to move the falling piece
/*
This function takes an integer parameter direction that specifies the direction in which the falling piece should be moved. A value of 1 means move down, 2 means move left, and 3 means move right.

The function first updates the position of the falling piece according to the specified direction. It then checks for collisions by iterating over each cell in the falling piece and checking if it collides with any occupied cell on the game board. If a collision is detected, the function locks the falling piece in place by copying its cells to the corresponding cells on the game board and generates a new piece. Otherwise, the function simply returns.
*/
int move_piece(Cmd direction)
{
    printf("%s(%d)\n", __func__, direction);
    // Move the piece in the specified direction
    if (direction == Cmd_Down)
    {
        // Move down
        if (collide(piece_y + 1, piece_x))
        {
            return 2;
        }
        else
        {
            piece_y++;
            return 1;
        }
    }
    else if (direction == Cmd_Left)
    {
        // Move left
        if (!collide(piece_y, piece_x - 1))
        {
            piece_x--;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (direction == Cmd_Right)
    {
        // Move right
        if (!collide(piece_y, piece_x + 1))
        {
            piece_x++;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        cerr << "Unknown move\n";
    }
    return 0;
}

bool collide(size_t piece_y, int piece_x)
{
    printf("%s(%lu,%d)\n", __func__, piece_y, piece_x);

    // // Collide floor
    // if (piece_y >= board.size())
    // {
    //     cerr << "Collide floor\n";
    //     return true;
    // }
    // Collide walls
    if (piece_x <= -3)
    {
        cerr << "Beyond left wall\n";
        return true;
    }
    // if (piece_x > board[0].size() - piece[0].size())
    // {
    //     cerr << "Collide wall\n";
    //     return true;
    // }
    
    // Check for collisions
    for (size_t i = 0; i < piece.size(); i++) {
        for (size_t j = 0; j < piece[i].size(); j++) {
            if (piece[i][j] != 0) {
                size_t x = piece_x + j;
                size_t y = piece_y + i;
                if (x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT || board[y][x] != 0) {
                    // Collision detected, lock the piece in place
                    cerr << "Collide piece(" << j << ',' << i << ") board(" << y << ',' << x << ")" << endl;
                    return true;
                }
            }
        }
    }
    return false;
}

void anchor()
{
    // Collision detected, lock the piece in place
    for (size_t i = 0; i < piece.size(); i++) {
        for (size_t j = 0; j < piece[i].size(); j++) {
            if (piece[i][j] != 0) {
                board[piece_y + i][piece_x + j] = piece[i][j];
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
    for (size_t i = 0; i < piece.size(); ++i)
    {
        for (size_t j = 0; j < piece[i].size(); ++j)
        {
            //reverse copy
            piece[i][piece[i].size() - j] = old_piece[i][j];
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

    printf("%s()\n", __func__);
    for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
        bool line_complete = true;
        for (size_t j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j] == 0) {
                line_complete = false;
                break;
            }
        }
        if (line_complete) {
            // Remove the completed line
            for (size_t j = 0; j < BOARD_WIDTH; j++) {
                board[i][j] = 0;
            }
            // Shift all the lines above it down
            for (int k = i - 1; k >= 0; k--) {
                for (size_t j = 0; j < BOARD_WIDTH; j++) {
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
    for (size_t i = 0; i < board.size(); i++) {

        for (size_t j = 0; j < board[i].size(); j++) {

            bool occupied = board[i][j];

            int li = i - piece_y;
            int lj = j - piece_x;

            if ((0 <= li && li < 4) && (0 <= lj && lj < 4))
            // if ((piece_y <= i && i < (piece_y + piece.size())) &&
            //     (piece_x <= j && j < (piece_x + piece[i - piece_y].size())))
            {
                // j - piece_x >= -0
                // j >= piece_x
                // piece_x <= j
                occupied |= piece[li][lj];
            }
            cout << (occupied ? 'o' : '.') << ' ';
        }
        cout << endl;
    }
}

// Function to print the game board
template<typename T>
void print_table(T const& table) {
    cout << "table\n";
    printf("piece(%lu,%d)\n", piece_y, piece_x);
    for (typename T::size_type i = 0; i < table.size(); i++) {

        for (typename T::size_type j = 0; j < table[i].size(); j++) {

            bool occupied = table[i][j];
            cout << (occupied ? 'o' : '.') << ' ';
        }
        cout << endl;
    }
}


#include <unistd.h>  //sleep

int main() {
    init_game();
    print_table(piece);
    Cmd cmd = Cmd_Down;

    // // Print the game board
    // print_board();

    while (true) {

        // Print the game board
        print_board();
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
        int moved = move_piece(cmd);
        if (moved == 0)
        {
            cout << "Lateral blocked\n";
        }
        else if (moved == 1)
        {
            cout << "Moved\n";
            if (cmd == Cmd_Down)
                piece_n_moves++;
            // Print the game board
            print_board();
        }
        else
        {
            // Fall arrested
            cout << "Fall arrested\n";
            anchor();
            if (piece_n_moves == 0)
            {
                cout << "Game over\n";
                return 0;
            }
            // Generate a new piece
            generate_piece();
        }

        // Check for completed lines
        check_lines();

        // // Generate a new piece if necessary
        // if (piece_y == 0) {
        //     generate_piece();
        // }
    }
    return 0;
}
