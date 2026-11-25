//
// Created by Vas Klu on 18.10.2026.
//

#ifndef INC_2048_BOARD_H
#define INC_2048_BOARD_H
#include <vector>


class Board {
private:
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    int target = DEFAULT_NUMBER_TO_WIN;
    std::vector<std::vector<int>> board;
    bool is_game_over = false;
    int score = 0;
    std::pair<int, int> last_spawned_tile{-1, -1};

    // Undo state
    std::vector<std::vector<int>> prev_board;
    int prev_score = 0;
    std::pair<int, int> prev_last_spawned_tile{-1, -1};
    bool has_undo_state = false;

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
    static constexpr int DEFAULT_NUMBER_TO_WIN = 2048;
    static constexpr int DEFAULT_WIDTH = 4;
    static constexpr int DEFAULT_HEIGHT = 4;
    static constexpr int TILES_ON_START_COUNT = 2;

    Board(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT, int target = DEFAULT_NUMBER_TO_WIN);

    int get_width() const;
    int get_height() const;
    int get_target() const;
    int get_tile(const std::pair<int, int> &coord) const;
    int get_tile(const int &x, const int &y) const;
    void set_tile(int value, const std::pair<int, int> &coord);
    bool get_is_game_over() const;
    bool has_winner() const;
    int get_score() const;
    std::pair<int, int> get_last_spawned_tile() const;

    void generate_board();
    void reset();
    bool undo();
    bool can_undo() const;

    bool move_up();
    bool move_right();
    bool move_down();
    bool move_left();

};


#endif //INC_2048_BOARD_H
