cmake_minimum_required(VERSION 3.18)

project(sdlapp C)

find_package(SDL2 REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

add_executable(main main.c blocks.c collisions.c render.c input.c)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(main SDL2 SDL2_image SDL2_ttf)
