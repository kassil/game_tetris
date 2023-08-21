#include "board.hpp"
#include <iostream>

using std::cerr;  using std::endl;

// Lock the piece in place
void anchor(board_type& board,
    piece_type const& piece, index_type const& pos)
{
    for (size_t i = 0; i < piece.size(); i++) {
        for (size_t j = 0; j < piece[i].size(); j++) {
            if (piece[i][j]) {
                board[pos.i + i][pos.j + j] = piece[i][j];
            }
        }
    }
}

bool collide(board_type const& board, piece_type const& piece, size_t piece_y, int piece_x)
{
    index_type const pos = {piece_y, piece_x};
    printf("%s(%lu,%d)\n", __func__, pos.i, pos.j);

    // Check for collisions
    for (size_t i = 0; i < piece.size(); i++) {
        const size_t y = pos.i + i;
        for (size_t j = 0; j < piece[i].size(); j++) {
            if (piece[i][j]) {
                const int x = pos.j + j;
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
