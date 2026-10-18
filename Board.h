//
// Created by Vas Klu on 18.10.2026.
//

#ifndef INC_2048_BOARD_H
#define INC_2048_BOARD_H
#include <vector>


class Board {
private:
    std::vector<std::vector<int>> board;
    bool is_game_over = false;

    void create_tile();
    bool check_game_over();
    int generate_value(const std::pair<int, int> &coord);

    bool move_tile_up(const int &x, const int &y, int &last_merged_x);
    bool move_tile_up(const std::pair<int, int> &coord, int &last_merged_x);
    bool move_tile_right(const int &x, const int &y, int &last_merged_y);
    bool move_tile_right(const std::pair<int, int> &coord, int &last_merged_y);
    bool move_tile_down(const int &x, const int &y, int &last_merged_x);
    bool move_tile_down(const std::pair<int, int> &coord, int &last_merged_x);
    bool move_tile_left(const int &x, const int &y, int &last_merged_y);
    bool move_tile_left(const std::pair<int, int> &coord, int &last_merged_y);

public:
    static constexpr int NUMBER_TO_WIN = 2048;
    static constexpr int WIDTH = 4;
    static constexpr int HEIGHT = 4;
    static constexpr int TILES_ON_START_COUNT = 2;

    Board();

    int get_tile(const std::pair<int, int> &coord) const;
    int get_tile(const int &x, const int &y);
    void set_tile(int value, const std::pair<int, int> &coord);
    bool get_is_game_over();
    bool has_winner();

    void generate_board();

    bool move_up();
    bool move_right();
    bool move_down();
    bool move_left();

};


#endif //INC_2048_BOARD_H
