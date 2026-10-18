cmake_minimum_required(VERSION 4.3)
project(2048)

set(CMAKE_CXX_STANDARD 20)

add_executable(2048 main.cpp
        Board.cpp
        Board.h
        Utils.cpp
        Utils.h
)
