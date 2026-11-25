cmake_minimum_required(VERSION 3.20)
project(2048)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main game executable
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

# Automated unit tests
enable_testing()

add_executable(unit_tests
        tests/unit_tests.cpp
        Board.cpp
        Board.h
        Utils.cpp
        Utils.h
        ArgParser.cpp
        ArgParser.h
)

target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME AllUnitTests COMMAND unit_tests)
