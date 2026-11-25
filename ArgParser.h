//
// Created by Vas Klu on 08.11.2026.
//

#ifndef INC_2048_ARGPARSER_H
#define INC_2048_ARGPARSER_H

#include <string>
#include <vector>

class ArgParser {
private:
    bool help_requested = false;
    bool has_error_flag = false;
    std::string error_message;

    int size = 4;
    int target = 2048;
    bool seed_set = false;
    unsigned int seed = 0;

    static bool is_power_of_two(int n);

public:
    ArgParser(int argc, char* argv[]);

    bool is_help_requested() const;
    bool has_error() const;
    std::string get_error_message() const;

    int get_size() const;
    int get_target() const;
    bool has_seed() const;
    unsigned int get_seed() const;

    void print_help() const;
};

#endif //INC_2048_ARGPARSER_H
