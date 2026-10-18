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

    void generate_board();
    void create_tile();
    bool check_game_over();
    int generate_value(const std::pair<int, int> &coord);
public:
    static constexpr int NUMBER_TO_WIN = 2048;
    static constexpr int WIDTH = 4;
    static constexpr int HEIGHT = 4;

    Board();

    int get_tile(const std::pair<int, int> &coord) const;
    void set_tile(int value, const std::pair<int, int> &coord);
    bool get_is_game_over();
    bool has_winner();

    void move_up();
    void move_right();
    void move_down();
    void move_left();


};


#endif //INC_2048_BOARD_H
