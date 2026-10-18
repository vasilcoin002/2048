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
    std::cout << "\n";
}

int main() {

    Board game_board;
    game_board.generate_board();

    printBoard(game_board);


    // game_board.move_left();
    // printBoard(game_board);
    game_board.move_right();
    printBoard(game_board);
    game_board.move_right();
    printBoard(game_board);


    return 0;
}
