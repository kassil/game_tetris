#pragma once
#include <array>
#include <stdlib.h>

typedef bool cell_type;

constexpr int    BOARD_WIDTH = 12;
constexpr size_t BOARD_HEIGHT = 12;
typedef std::array<std::array<cell_type, BOARD_WIDTH>, BOARD_HEIGHT> board_type;

typedef std::array<std::array<cell_type, 4>, 4> piece_type;

template <typename value_type, std::size_t N, std::size_t M>
void rotate_array(std::array<std::array<value_type, M>, N>& arr);

template <typename value_type, std::size_t N, std::size_t M>
void transpose_array(std::array<std::array<value_type, M>, N>& arr);

// Rotate 2D std::array by 90 degrees
template <typename value_type, std::size_t N, std::size_t M>
void rotate_array(std::array<std::array<value_type, M>, N>& arr) {
    // Transpose the array
    transpose_array(arr);
    // Reverse each row of the transposed array
    for (std::size_t i = 0; i < N; i++) {
        for (std::size_t j = 0; j < M/2; j++) {
            std::swap(arr[i][j], arr[i][M-j-1]);
        }
    }
}

// Rotate 2D std::array by 90 degrees
template <typename value_type, std::size_t N, std::size_t M>
void transpose_array(std::array<std::array<value_type, M>, N>& arr) {
    // Transpose the array
    for (std::size_t i = 0; i < N; i++) {
        for (std::size_t j = i; j < M; j++) {
            std::swap(arr[i][j], arr[j][i]);
        }
    }
}

struct index_type
{
    size_t i;
    int j;
};
