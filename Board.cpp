//
// Created by Vas Klu on 18.10.2026.
//

#include "Board.h"
#include "Utils.h"

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

bool Board::move_tile_up(const int &x, const int &y, int &last_merged_x) {
    if (x <= 0) return false;

    int move_to = x;
    for (int to = x - 1; to >= 0; to--) {
        if (board[to][y] == 0) move_to = to;
        else if (board[to][y] != board[x][y] || to == last_merged_x) break;

        // ... joining two tiles into one ...
        else {
            board[to][y] *= 2;
            board[x][y] = 0;
            last_merged_x = to;
            return true;
        }
    }

    // ... moving to empty space logic ...
    if (move_to != x) {
        board[move_to][y] = board[x][y];
        board[x][y] = 0;
        return true;
    }
    return false;
}

bool Board::move_tile_up(const std::pair<int, int> &coord, int &last_merged_x) {
    return move_tile_up(coord.first, coord.second, last_merged_x);
}

bool Board::move_tile_right(const int &x, const int &y, int &last_merged_y) {
    if (y >= WIDTH - 1) return false;

    int move_to = y;
    for (int to = y + 1; to < WIDTH; to++) {
        if (board[x][to] == 0) move_to = to;
        else if (board[x][to] != board[x][y] || to == last_merged_y) break;

        // ... joining two tiles into one ...
        else {
            board[x][to] *= 2;
            board[x][y] = 0;
            last_merged_y = to;
            return true;
        }
    }

    // ... moving to empty space logic ...
    if (move_to != y) {
        board[x][move_to] = board[x][y];
        board[x][y] = 0;
        return true;
    }
    return false;
}

bool Board::move_tile_right(const std::pair<int, int> &coord, int &last_merged_y) {
    return move_tile_right(coord.first, coord.second, last_merged_y);
}

bool Board::move_tile_down(const int &x, const int &y, int &last_merged_x) {
    if (x >= HEIGHT - 1) return false;

    int move_to = x;
    for (int to = x + 1; to < HEIGHT; to++) {
        if (board[to][y] == 0) move_to = to;
        else if (board[to][y] != board[x][y] || to == last_merged_x) break;

        // ... joining two tiles into one ...
        else {
            board[to][y] *= 2;
            board[x][y] = 0;
            last_merged_x = to;
            return true;
        }
    }

    // ... moving to empty space logic ...
    if (move_to != x) {
        board[move_to][y] = board[x][y];
        board[x][y] = 0;
        return true;
    }
}

bool Board::move_tile_down(const std::pair<int, int> &coord, int &last_merged_x) {
    return move_tile_down(coord.first, coord.second, last_merged_x);
}

bool Board::move_tile_left(const int &x, const int &y, int &last_merged_y) {
    if (y <= 0) return false;

    int move_to = y;
    for (int to = y - 1; to >= 0; to--) {
        if (board[x][to] == 0) move_to = to;
        else if (board[x][to] != board[x][y] || to == last_merged_y) break;

        // ... joining two tiles into one ...
        else {
            board[x][to] *= 2;
            board[x][y] = 0;
            last_merged_y = to;
            return true;
        }
    }

    // ... moving to empty space logic ...
    if (move_to != y) {
        board[x][move_to] = board[x][y];
        board[x][y] = 0;
        return true;
    }
    return false;
}

bool Board::move_tile_left(const std::pair<int, int> &coord, int &last_merged_y) {
    return move_tile_left(coord.first, coord.second, last_merged_y);
}

Board::Board() {
    board = std::vector<std::vector<int>>(HEIGHT, std::vector<int>(WIDTH, 0));
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

    // ... defining winner logic ...
}


void Board::generate_board() {
    for (int i = 0; i < TILES_ON_START_COUNT; i++) {
        // int x = Utils::randint(0, WIDTH - 1);
        // int y = Utils::randint(0, HEIGHT - 1);
        // generate_value(std::make_pair(x, y));
        create_tile();
    }
    // set_tile(2, std::make_pair(0, 0));
    // set_tile(2, std::make_pair(1, 0));
    // set_tile(4, std::make_pair(2, 0));
}


bool Board::move_up() {
    if (is_game_over) return false;

    bool board_changed = false;

    // ... moving logic ...
    for (int y = 0; y < WIDTH; y++) {
        int last_merged_x = -1;

        for (int x = 1; x < HEIGHT; x++) {
            if (get_tile(x, y) == 0) continue;

            if (move_tile_up(x, y, last_merged_x)) {
                board_changed = true;
            }
        }
    }

    if (board_changed) {
        create_tile();
    }
    return board_changed;
}

bool Board::move_right() {
    if (is_game_over) return false;

    bool board_changed = false;

    // ... moving logic ...
    for (int x = 0; x < HEIGHT; x++) {
        int last_merged_y = -1;

        for (int y = WIDTH - 2; y >= 0; y--) {
            if (get_tile(x, y) == 0) continue;

            if (move_tile_right(x, y, last_merged_y)) {
                board_changed = true;
            }
        }
    }

    if (board_changed) {
        create_tile();
    }
    return board_changed;
}

bool Board::move_down() {
    if (is_game_over) return false;

    bool board_changed = false;

    // ... moving logic ...
    for (int y = 0; y < WIDTH; y++) {
        int last_merged_x = -1;

        for (int x = HEIGHT - 2; x >= 0; x--) {
            if (get_tile(x, y) == 0) continue;

            if (move_tile_down(x, y, last_merged_x)) {
                board_changed = true;
            }
        }
    }

    if (board_changed) {
        create_tile();
    }
    return board_changed;
}

bool Board::move_left() {
    if (is_game_over) return false;

    bool board_changed = false;

    // ... moving logic ...
    for (int x = 0; x < HEIGHT; x++) {
        int last_merged_y = -1;

        for (int y = 1; y < WIDTH; y++) {
            if (get_tile(x, y) == 0) continue;

            if (move_tile_left(x, y, last_merged_y)) {
                board_changed = true;
            }
        }
    }

    if (board_changed) {
        create_tile();
    }
    return board_changed;
}
