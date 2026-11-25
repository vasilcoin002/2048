//
// Created by Vas Klu on 08.11.2026.
//

#include "ArgParser.h"
#include <iostream>
#include <stdexcept>

bool ArgParser::is_power_of_two(int n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

ArgParser::ArgParser(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            help_requested = true;
            return;
        } else if (arg == "-s" || arg == "--size") {
            if (i + 1 >= argc) {
                has_error_flag = true;
                error_message = "Error: Missing value for " + arg;
                return;
            }
            try {
                size = std::stoi(argv[++i]);
                if (size < 3 || size > 8) {
                    has_error_flag = true;
                    error_message = "Error: Size must be between 3 and 8.";
                    return;
                }
            } catch (...) {
                has_error_flag = true;
                error_message = "Error: Invalid number for " + arg;
                return;
            }
        } else if (arg == "-t" || arg == "--target") {
            if (i + 1 >= argc) {
                has_error_flag = true;
                error_message = "Error: Missing value for " + arg;
                return;
            }
            try {
                target = std::stoi(argv[++i]);
                if (target < 8 || !is_power_of_two(target)) {
                    has_error_flag = true;
                    error_message = "Error: Target must be a power of 2 (at least 8).";
                    return;
                }
            } catch (...) {
                has_error_flag = true;
                error_message = "Error: Invalid number for " + arg;
                return;
            }
        } else if (arg == "--seed") {
            if (i + 1 >= argc) {
                has_error_flag = true;
                error_message = "Error: Missing value for --seed";
                return;
            }
            try {
                seed = static_cast<unsigned int>(std::stoul(argv[++i]));
                seed_set = true;
            } catch (...) {
                has_error_flag = true;
                error_message = "Error: Invalid number for --seed";
                return;
            }
        } else {
            has_error_flag = true;
            error_message = "Error: Unrecognized option '" + arg + "'";
            return;
        }
    }
}

bool ArgParser::is_help_requested() const {
    return help_requested;
}

bool ArgParser::has_error() const {
    return has_error_flag;
}

std::string ArgParser::get_error_message() const {
    return error_message;
}

int ArgParser::get_size() const {
    return size;
}

int ArgParser::get_target() const {
    return target;
}

bool ArgParser::has_seed() const {
    return seed_set;
}

unsigned int ArgParser::get_seed() const {
    return seed;
}

void ArgParser::print_help() const {
    std::cout << "2048 Console Game - Semester Project (B6B36PCC)\n";
    std::cout << "Usage: 2048 [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help            Show this help message and exit\n";
    std::cout << "  -s, --size <3..8>     Board dimensions (NxN grid, default: 4)\n";
    std::cout << "  -t, --target <N>      Winning tile target (power of 2 >= 8, default: 2048)\n";
    std::cout << "  --seed <NUMBER>       Fixed RNG seed for deterministic testing\n\n";
    std::cout << "Controls:\n";
    std::cout << "  W / Up Arrow          Move tiles UP\n";
    std::cout << "  S / Down Arrow        Move tiles DOWN\n";
    std::cout << "  A / Left Arrow        Move tiles LEFT\n";
    std::cout << "  D / Right Arrow       Move tiles RIGHT\n";
    std::cout << "  U                     Undo the last move\n";
    std::cout << "  R                     Restart the game\n";
    std::cout << "  Q / Esc               Quit the game\n\n";
    std::cout << "Visuals:\n";
    std::cout << "  Tiles are color-coded by number, newly spawned tiles are highlighted in green.\n\n";
    std::cout << "Objective:\n";
    std::cout << "  Slide numbered tiles on the grid. When two tiles with the same\n";
    std::cout << "  number touch, they merge into one with double the value.\n";
    std::cout << "  Reach the target tile to win!\n";
}
