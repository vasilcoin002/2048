//
// Created by Vas Klu on 18.10.2026.
//

#include "Board.h"
#include "Utils.h"

void Board::generate_board() {
    int tiles_count = Utils::randint(2, 8);

    for (int i = 0; i < tiles_count; i++) {
        int x = Utils::randint(0, 3);
        int y = Utils::randint(0, 3);
        int val = std::pow(2, Utils::randint(1, 2));

        set_tile(val, std::make_pair(x, y));
    }
}

void Board::create_tile() {

}

bool Board::check_game_over() {

}

Board::Board() {
    this->generate_board();
}

int Board::get_tile(const std::pair<int, int> &coords) const {
    return this->board[coords.first][coords.second];
}

void Board::set_tile(const int value, const std::pair<int, int> &coords) {
    this->board[coords.first][coords.second] = value;
}

bool Board::get_is_game_over() {
    return is_game_over;
}

bool Board::has_winner() {
    if (!is_game_over) return false;
}

int Board::get_number_to_win() {
    return NUMBER_TO_WIN;
}

void Board::move_up() {
    if (is_game_over) return;

    // ... moving logic ...

    create_tile();
    check_game_over();
}

void Board::move_right() {
    if (is_game_over) return;

    // ... moving logic ...

    create_tile();
    check_game_over();
}

void Board::move_down() {
    if (is_game_over) return;

    // ... moving logic ...

    create_tile();
    check_game_over();
}

void Board::move_left() {
    if (is_game_over) return;

    // ... moving logic ...

    create_tile();
    check_game_over();
}
