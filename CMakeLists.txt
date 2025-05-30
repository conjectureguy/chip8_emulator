cmake_minimum_required(VERSION 3.31)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(chip8_emulator
        src/Main.cpp
        src/Chip8.cpp
        include/Chip8.h)
