#include "ArgParser.h"
#include "Game.h"

int main(int argc, char* argv[]) {
    ArgParser parser(argc, argv);

    if (parser.is_help_requested()) {
        parser.print_help();
        return 0;
    }

    Game game;
    game.run();

    return 0;
}
