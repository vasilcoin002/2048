//
// Created by Vas Klu on 19.10.2026.
//

#include <iostream>
#include "BoardPrinter.h"

int BoardPrinter::getValueLength(int val) const {
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

int BoardPrinter::computeCellNumWidth() const {
    int max_val = board.get_target();
    for (int i = 0; i < board.get_height(); ++i) {
        for (int j = 0; j < board.get_width(); ++j) {
            int v = board.get_tile(i, j);
            if (v > max_val) {
                max_val = v;
            }
        }
    }
    return std::max(4, getValueLength(max_val));
}

// print number with whitespaces
void BoardPrinter::printValueWithWhitespaces(std::ostream& out, int val, int max_num_length, int whitespace) const {

    for (int i = 0; i < whitespace; i++) {
        out << " ";
    }

    const int length = getValueLength(val);
    const int padding_left = (max_num_length > length) ? (max_num_length - length) : 0;

    for (int k = 0; k < padding_left; k++) {
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

void BoardPrinter::printDashesLine(std::ostream& out, const int length) const {
    for (int i = 0; i < length; i++) {
        out << '-';
    }
    out << "\n";
}

const char* BoardPrinter::getTileColor(int val, bool is_new) {
    if (val == 0) return "";
    if (is_new) return "\033[1;92m"; // Bold green for newly spawned tile

    switch (val) {
        case 2:    return "\033[37m";   // White / Light gray
        case 4:    return "\033[96m";   // Bright Cyan
        case 8:    return "\033[94m";   // Bright Blue
        case 16:   return "\033[93m";   // Bright Yellow
        case 32:   return "\033[33m";   // Orange / Yellow
        case 64:   return "\033[91m";   // Bright Red
        case 128:  return "\033[31m";   // Dark Red
        case 256:  return "\033[95m";   // Bright Magenta
        case 512:  return "\033[35m";   // Magenta
        case 1024: return "\033[1;36m"; // Bold Cyan
        case 2048: return "\033[1;93m"; // Bold Golden Yellow
        default:   return "\033[1;95m"; // Bold Bright Magenta for > 2048
    }
}

BoardPrinter::BoardPrinter(Board &board)
    : board(board), NUM_WIDTH(std::max(4, getValueLength(board.get_target()))) {}

void BoardPrinter::printBoard(std::ostream& out) {
    const int b_width = board.get_width();
    const int b_height = board.get_height();
    const int cell_width = computeCellNumWidth();

    // numbers with whitespaces + boundaries
    const int line_width = b_width * (cell_width + TILE_PADDING * 2) + (b_width + 1);
    const auto last_spawned = this->board.get_last_spawned_tile();

    printDashesLine(out, line_width);

    // print "| num | num2 | ... |"
    for (int i = 0; i < b_height; i++) {
        for (int j = 0; j < b_width; j++) {
            out << "|";

            const int val = this->board.get_tile(i, j);
            const bool is_new = (val != 0 && std::make_pair(i, j) == last_spawned);

            if (val != 0) {
                out << getTileColor(val, is_new);
            }

            printValueWithWhitespaces(out, val, cell_width, TILE_PADDING);

            if (val != 0) {
                out << "\033[0m";
            }
        }
        out << "|\n";
        printDashesLine(out, line_width);
    }

}