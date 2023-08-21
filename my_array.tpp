#pragma once
#include <array>
#include <stdlib.h>

typedef bool cell_type;

constexpr int    BOARD_WIDTH = 12;
constexpr size_t BOARD_HEIGHT = 12;
typedef std::array<std::array<cell_type, BOARD_WIDTH>, BOARD_HEIGHT> board_type;

typedef std::array<std::array<cell_type, 4>, 4> piece_type;

template <std::size_t N, std::size_t M>
void rotate_array(std::array<std::array<cell_type, M>, N>& arr);

template <std::size_t N, std::size_t M>
void transpose_array(std::array<std::array<cell_type, M>, N>& arr);

struct index_type
{
    size_t i;
    int j;
};
