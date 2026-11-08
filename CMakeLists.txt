cmake_minimum_required(VERSION 3.20)
project(2048)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(2048
        main.cpp
        Board.cpp
        Board.h
        Utils.cpp
        Utils.h
        BoardPrinter.cpp
        BoardPrinter.h
        Game.cpp
        Game.h
        ArgParser.cpp
        ArgParser.h
)
