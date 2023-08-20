# Tetris Game

This is a simple implementation of the classic Tetris game in C++. It features a basic game loop, a falling piece generator, collision detection, and line completion detection.

## Build Instructions

To build this project, you will need CMake and a C++ compiler installed on your system.

1. Clone the repository or download the source code.
2. Create a build directory inside the project directory: `mkdir build`
3. Navigate to the build directory: `cd build`
4. Run CMake to generate the Makefile: `cmake ..`
5. Run `make` to build the project: `make`

The executable `myapp` will be located in the `build` directory.

## How to Play

To play the game, simply run the executable file `tetris.exe`. The game will start automatically, and you can control the falling pieces using the following keys:

- `LEFT ARROW`: move the piece left
- `RIGHT ARROW`: move the piece right
- `DOWN ARROW`: move the piece down faster
- `UP ARROW`: rotate the piece

The goal of the game is to complete as many lines as possible by filling them with falling pieces. Each completed line will be removed, and you will score points for each completed line.

## Requirements

To compile and run the game, you will need a C++11-compatible compiler, such as GCC or Clang.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

