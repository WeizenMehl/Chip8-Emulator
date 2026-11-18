
cmake_minimum_required(VERSION 3.16)

project(chip8 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Find SDL2
find_package(SDL2 REQUIRED)

# Source files
add_executable(chip8
    main.c
)

# Link SDL2
target_link_libraries(chip8 PRIVATE SDL2::SDL2)
