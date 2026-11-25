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

    int getValueLength(int val) const;
    void printValueWithWhitespaces(std::ostream& out, int val, int max_num_length, int whitespace) const;
    void printDashesLine(std::ostream& out, int length) const;
    static const char* getTileColor(int val, bool is_new);
public:
    int NUM_WIDTH = 4;
    static constexpr int TILE_PADDING = 1;

    BoardPrinter(Board &board);

    int computeCellNumWidth() const;
    void printBoard(std::ostream& out);
};


#endif //INC_2048_BOARDPRINTER_H
