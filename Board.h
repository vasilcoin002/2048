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
public:

    Board();

    int get_tile(const std::pair<int, int> &coords) const;
    void set_tile(int value, const std::pair<int, int> &coords);

    void move_up();
    void move_right();
    void move_down();
    void move_left();

    bool get_is_game_over();
    bool has_winner();
};


#endif //INC_2048_BOARD_H
