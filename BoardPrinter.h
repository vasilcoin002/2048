//
// Created by Vas Klu on 19.10.2026.
//

#ifndef INC_2048_BOARDPRINTER_H
#define INC_2048_BOARDPRINTER_H
#include <iostream>
#include "Board.h"


class BoardPrinter {
private:
    Board &board;

    int getValueLength(int val);
    void printValueWithWhitespaces(std::ostream& out, int val, int max_num_length, int whitespace);
    void printDashesLine(std::ostream& out, int length);
public:
    const int NUM_WIDTH;
    static constexpr int TILE_PADDING = 1;

    BoardPrinter(Board &board);

    void printBoard(std::ostream& out);
};


#endif //INC_2048_BOARDPRINTER_H
