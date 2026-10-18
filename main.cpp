#include <iostream>

#include "Board.h"

void printBoard(const Board& board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int val = board.get_tile(std::make_pair(i, j));
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}

int main() {

    Board game_board;

    printBoard(game_board);

    return 0;
}
