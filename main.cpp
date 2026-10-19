#include <iostream>

#include "Board.h"
#include "BoardPrinter.h"

// TODO add tests
// TODO add controlling from the keyboard
int main() {

    Board game_board;
    game_board.generate_board();

    BoardPrinter board_printer(game_board);

    board_printer.printBoard(std::cout);

    while (!game_board.get_is_game_over()) {

        std::cout << "^\n";
        game_board.move_up();
        board_printer.printBoard(std::cout);


        std::cout << "->\n";
        game_board.move_right();
        board_printer.printBoard(std::cout);


        std::cout << "!\n";
        game_board.move_down();
        board_printer.printBoard(std::cout);


        std::cout << "<-\n";
        game_board.move_left();
        board_printer.printBoard(std::cout);

    }

    return 0;
}
