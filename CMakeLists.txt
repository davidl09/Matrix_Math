cmake_minimum_required(VERSION 3.22)
project(Matrix_Math C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Matrix_Math main.c matrix.c matrix.h)

target_link_libraries(Matrix_Math -lm)
