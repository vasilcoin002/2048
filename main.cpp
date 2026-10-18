#include <iostream>

#include "Board.h"

void printBoard(const Board& board) {
    for (int i = 0; i < Board::WIDTH; i++) {
        for (int j = 0; j < Board::HEIGHT; j++) {
            int val = board.get_tile(std::make_pair(i, j));
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}

int main() {

    Board game_board;

    printBoard(game_board);

    for (int i = 0; i < Board::HEIGHT * Board::WIDTH - 1; i++) {
        game_board.move_down();
        std::cout << "\n";
        printBoard(game_board);
    }

    game_board.move_down();

    return 0;
}
