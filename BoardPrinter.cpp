//
// Created by Vas Klu on 19.10.2026.
//

#include <iostream>
#include "BoardPrinter.h"

int BoardPrinter::getValueLength(int val) {
    int length = val < 0 ? 1 : 0;

    if (val == 0) {
        return 1;
    }

    while (val != 0) {
        val /= 10;
        length++;
    }

    return length;
}

// print number with whitespaces
void BoardPrinter::printValueWithWhitespaces(std::ostream& out, int val, int max_num_length, int whitespace) {

    for (int i = 0; i < whitespace; i++) {
        out << " ";
    }

    const int length = getValueLength(val);

    for (int k = 0; k < max_num_length - length; k++) {
        out << " ";
    }

    if (val != 0) {
        out << val;
    } else {
        out << " ";
    }

    for (int i = 0; i < whitespace; i++) {
        out << " ";
    }
}

void BoardPrinter::printDashesLine(std::ostream& out, const int length) {
    for (int i = 0; i < length; i++) {
        out << '-';
    }
    out << "\n";
}

BoardPrinter::BoardPrinter(Board &board)
    : board(board), NUM_WIDTH(getValueLength(Board::NUMBER_TO_WIN)) {}

void BoardPrinter::printBoard(std::ostream& out) {

    // numbers with whitespaces + boundaries
    const int width = Board::WIDTH * (NUM_WIDTH + TILE_PADDING * 2) + (Board::WIDTH + 1);
    const auto last_spawned = this->board.get_last_spawned_tile();

    printDashesLine(out, width);

    // print "| num | num2 | ... |"
    for (int i = 0; i < Board::HEIGHT; i++) {
        for (int j = 0; j < Board::WIDTH; j++) {
            out << "|";

            const int val = this->board.get_tile(i, j);
            const bool is_new = (val != 0 && std::make_pair(i, j) == last_spawned);

            if (is_new) {
                out << "\033[1;92m"; // Highlight new tile: bold green text, standard background
            }

            printValueWithWhitespaces(out, val, NUM_WIDTH, TILE_PADDING);

            if (is_new) {
                out << "\033[0m";
            }
        }
        out << "|\n";
        printDashesLine(out, width);
    }

}