//
// Created by Vas Klu on 18.10.2026.
//

#ifndef INC_2048_BOARD_H
#define INC_2048_BOARD_H
#include <vector>


class Board {
private:
    std::vector<std::vector<int>> board = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };;
    bool is_game_over = false;
    static constexpr int NUMBER_TO_WIN = 2048;

    void generate_board();
    void create_tile();
    bool check_game_over();
public:

    Board();

    int get_tile(const std::pair<int, int> &coords) const;
    void set_tile(int value, const std::pair<int, int> &coords);
    bool get_is_game_over();
    bool has_winner();
    int get_number_to_win();

    void move_up();
    void move_right();
    void move_down();
    void move_left();


};


#endif //INC_2048_BOARD_H
