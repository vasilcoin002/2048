#include <iostream>
#include "ArgParser.h"
#include "Game.h"
#include "Utils.h"

int main(int argc, char* argv[]) {
    ArgParser parser(argc, argv);

    if (parser.is_help_requested()) {
        parser.print_help();
        return 0;
    }

    if (parser.has_error()) {
        std::cerr << parser.get_error_message() << "\n";
        std::cerr << "Run with --help for usage information.\n";
        return 1;
    }

    if (parser.has_seed()) {
        Utils::set_seed(parser.get_seed());
    }

    int size = parser.get_size();
    int target = parser.get_target();

    Game game(size, size, target);
    game.run();

    return 0;
}
