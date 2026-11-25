//
// Created by Vas Klu on 08.11.2026.
//

#ifndef INC_2048_GAME_H
#define INC_2048_GAME_H

#include "Board.h"
#include "BoardPrinter.h"

class Game {
private:
    Board board;
    BoardPrinter printer;
    bool is_running{true};
    int moves_count{0};

    void set_raw_mode(bool enable);
    void render();
    void process_input();

public:
    Game();
    ~Game();

    void run();
};

#endif //INC_2048_GAME_H
