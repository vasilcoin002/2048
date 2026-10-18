//
// Created by Vas Klu on 18.10.2026.
//

#include "Board.h"

void Board::generate_board() {

}

void Board::create_tile() {

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

void Board::move_up() {

}

void Board::move_right() {

}

void Board::move_down() {

}

void Board::move_left() {

}

bool Board::get_is_game_over() {

}

bool Board::has_winner() {

}
