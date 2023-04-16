cmake_minimum_required(VERSION 3.23)
project(Matrix_Math C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Matrix_Math
        main.c matrix.c matrix.h)
