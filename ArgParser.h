//
// Created by Vas Klu on 08.11.2026.
//

#ifndef INC_2048_ARGPARSER_H
#define INC_2048_ARGPARSER_H

#include <string>
#include <vector>

class ArgParser {
private:
    std::vector<std::string> args;
    bool help_requested = false;

public:
    ArgParser(int argc, char* argv[]);

    bool is_help_requested() const;
    void print_help() const;
};

#endif //INC_2048_ARGPARSER_H
