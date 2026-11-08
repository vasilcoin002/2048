//
// Created by Vas Klu on 08.11.2026.
//

#include "ArgParser.h"
#include <iostream>

ArgParser::ArgParser(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        args.push_back(arg);

        if (arg == "--help" || arg == "-h") {
            help_requested = true;
        }
    }
}

bool ArgParser::is_help_requested() const {
    return help_requested;
}

void ArgParser::print_help() const {
    std::cout << "2048 Console Game - Semester Project (B6B36PCC)\n";
    std::cout << "Usage: 2048 [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help    Show this help message and exit\n\n";
    std::cout << "Controls:\n";
    std::cout << "  W / Up Arrow      Move tiles UP\n";
    std::cout << "  S / Down Arrow    Move tiles DOWN\n";
    std::cout << "  A / Left Arrow    Move tiles LEFT\n";
    std::cout << "  D / Right Arrow   Move tiles RIGHT\n";
    std::cout << "  R                 Restart the game\n";
    std::cout << "  Q / Esc           Quit the game\n\n";
    std::cout << "Visuals:\n";
    std::cout << "  Newly spawned tiles are highlighted with green text.\n\n";
    std::cout << "Objective:\n";
    std::cout << "  Slide numbered tiles on the 4x4 grid. When two tiles with the same\n";
    std::cout << "  number touch, they merge into one with double the value.\n";
    std::cout << "  Reach tile 2048 to win!\n";
}
