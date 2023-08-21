#pragma once
#include "my_array.tpp"
#include <array>

void anchor(board_type& board, piece_type const& piece, index_type const& piece_pos);
bool collide(board_type const& board, piece_type const& piece, size_t piece_y, int piece_x);
