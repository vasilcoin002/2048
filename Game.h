//
// Created by Vas Klu on 08.11.2026.
//

#ifndef INC_2048_GAME_H
#define INC_2048_GAME_H

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include "Board.h"
#include "BoardPrinter.h"

enum class Command {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    RESTART,
    QUIT,
    NONE
};

struct GameState {
    std::atomic<bool> is_running{true};
    int moves_count{0};
    int game_time_seconds{0};

    // inputThread -> computeThread
    std::queue<Command> input_queue;
    std::mutex input_mutex;
    std::condition_variable input_cv;

    // computeThread -> outputThread
    bool needs_render{true};
    std::mutex render_mutex;
    std::condition_variable render_cv;
};

class Game {
private:
    Board board;
    BoardPrinter printer;
    GameState state;

    std::thread input_thread;
    std::thread compute_thread;
    std::thread output_thread;

    void set_raw_mode(bool enable);
    void input_loop();
    void compute_loop();
    void output_loop();

public:
    Game();
    ~Game();

    void run();
};

#endif //INC_2048_GAME_H
