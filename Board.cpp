//
// Created by Vas Klu on 18.10.2026.
//

#include "Board.h"
#include "Utils.h"

void Board::generate_board() {
    int tiles_count = Utils::randint(2, 8);

    for (int i = 0; i < tiles_count; i++) {
        int x = Utils::randint(0, WIDTH - 1);
        int y = Utils::randint(0, HEIGHT - 1);
        generate_value(std::make_pair(x, y));
    }
}

int Board::generate_value(const std::pair<int, int>& coord) {
    int value = std::pow(2, Utils::randint(1, 2));

    set_tile(value, coord);

    return value;
}

void Board::create_tile() {
    if (is_game_over) return;

    std::vector<std::pair<int, int>> free_coords = {};

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (board[i][j] == 0) {
                free_coords.emplace_back(i, j);
            }
        }
    }
    if (free_coords.empty()) {
        is_game_over = true;
        return;
    }

    generate_value(free_coords[Utils::randint(0, free_coords.size() - 1)]);

    check_game_over();
}

bool Board::check_game_over() {
    if (is_game_over) return true;

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            int tile = get_tile(x, y);

            // check if it's an empty tile
            if (tile == 0) {
                return false;
            }

            // check if neighbour has same value (to possibly connect them together)
            if (
                x > 0           && tile == get_tile(x-1, y) ||
                x < WIDTH - 1   && tile == get_tile(x+1, y) ||
                y > 0           && tile == get_tile(x, y-1) ||
                y < HEIGHT - 1  && tile == get_tile(x, y+1)
            ) {
                return false;
            }
        }
    }

    is_game_over = true;
    return true;
}

Board::Board() {
    board = std::vector<std::vector<int>>(HEIGHT, std::vector<int>(WIDTH, 0));
    // this->generate_board();
}

int Board::get_tile(const std::pair<int, int> &coord) const {
    return this->board[coord.first][coord.second];
}

int Board::get_tile(const int &x, const int &y) {
    return this->board[x][y];
}

void Board::set_tile(const int value, const std::pair<int, int> &coord) {
    this->board[coord.first][coord.second] = value;
}

bool Board::get_is_game_over() {
    return is_game_over;
}

bool Board::has_winner() {
    if (!is_game_over) return false;
}


void Board::move_up() {
    if (is_game_over) return;

    // ... moving logic ...

    create_tile();
}

void Board::move_right() {
    if (is_game_over) return;

    // ... moving logic ...

    create_tile();
}

void Board::move_down() {
    if (is_game_over) return;

    // ... moving logic ...

    create_tile();
}

void Board::move_left() {
    if (is_game_over) return;

    // ... moving logic ...

    create_tile();
}
