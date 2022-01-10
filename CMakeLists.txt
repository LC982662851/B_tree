cmake_minimum_required(VERSION 3.20)
project(wan0 C)

set(CMAKE_C_STANDARD 99)

add_executable(wan0 main.c btree.c btree.h)
