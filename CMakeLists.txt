cmake_minimum_required(VERSION 3.31)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_executable(chip8_emulator
    src/Main.cpp
    src/Chip8.cpp
    src/glad.c
    src/Platform.cpp
    include/Chip8.h
    include/Platform.h
    include/glad/glad.h
    include/KHR/khrplatform.h
)

# Include directories for SDL2 and glad
target_include_directories(chip8_emulator PRIVATE
    ${SDL2_INCLUDE_DIRS}
    include          # your project headers
)


target_link_libraries(chip8_emulator PRIVATE
    ${SDL2_LIBRARIES}
)
