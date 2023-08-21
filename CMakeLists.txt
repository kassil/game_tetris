cmake_minimum_required(VERSION 3.0)

project(tetris)

# Get a list of all the .cpp files in the current directory
file(GLOB SOURCES *.cpp)

# Add source file to the project
add_executable(${PROJECT_NAME} ${SOURCES})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
endif()
