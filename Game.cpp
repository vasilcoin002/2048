//
// Created by Vas Klu on 08.11.2026.
//

#include "Game.h"
#include <iostream>
#include <iomanip>
#include <termios.h>
#include <unistd.h>

Game::Game() : printer(board) {
    board.generate_board();
}

Game::~Game() {
    state.is_running.store(false);
    set_raw_mode(false);

    state.input_cv.notify_all();
    state.render_cv.notify_all();

    if (input_thread.joinable()) input_thread.join();
    if (compute_thread.joinable()) compute_thread.join();
    if (output_thread.joinable()) output_thread.join();
}

void Game::set_raw_mode(bool enable) {
    static struct termios orig_termios;
    static bool is_raw = false;

    if (!isatty(STDIN_FILENO)) {
        return;
    }

    if (enable && !is_raw) {
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_cc[VMIN] = 1;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        is_raw = true;
    } else if (!enable && is_raw) {
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
        is_raw = false;
    }
}

void Game::input_loop() {
    set_raw_mode(true);

    while (state.is_running.load()) {
        char ch = 0;
        ssize_t bytes_read = read(STDIN_FILENO, &ch, 1);

        if (bytes_read <= 0) {
            // EOF or error (e.g. redirected input ended)
            std::lock_guard<std::mutex> lock(state.input_mutex);
            state.input_queue.push(Command::QUIT);
            state.input_cv.notify_one();
            break;
        }

        Command cmd = Command::NONE;

        if (ch == 'q' || ch == 'Q') {
            cmd = Command::QUIT;
        } else if (ch == 'r' || ch == 'R') {
            cmd = Command::RESTART;
        } else if (ch == 'w' || ch == 'W') {
            cmd = Command::UP;
        } else if (ch == 's' || ch == 'S') {
            cmd = Command::DOWN;
        } else if (ch == 'a' || ch == 'A') {
            cmd = Command::LEFT;
        } else if (ch == 'd' || ch == 'D') {
            cmd = Command::RIGHT;
        } else if (ch == '\033') { // Escape sequence (e.g. arrow keys)
            char seq[2] = {0, 0};
            if (read(STDIN_FILENO, &seq[0], 1) > 0 && seq[0] == '[') {
                if (read(STDIN_FILENO, &seq[1], 1) > 0) {
                    switch (seq[1]) {
                        case 'A': cmd = Command::UP; break;
                        case 'B': cmd = Command::DOWN; break;
                        case 'C': cmd = Command::RIGHT; break;
                        case 'D': cmd = Command::LEFT; break;
                        default: break;
                    }
                }
            } else {
                cmd = Command::QUIT; // Solo Esc quits game
            }
        }

        if (cmd != Command::NONE) {
            {
                std::lock_guard<std::mutex> lock(state.input_mutex);
                state.input_queue.push(cmd);
            }
            state.input_cv.notify_one();

            if (cmd == Command::QUIT) {
                break;
            }
        }
    }

    set_raw_mode(false);
}

void Game::compute_loop() {
    using namespace std::chrono_literals;
    auto last_timer_tick = std::chrono::steady_clock::now();

    while (state.is_running.load()) {
        std::unique_lock<std::mutex> lock(state.input_mutex);

        state.input_cv.wait_for(lock, 200ms, [this]() {
            return !state.input_queue.empty() || !state.is_running.load();
        });

        if (!state.is_running.load()) {
            break;
        }

        bool state_changed = false;

        while (!state.input_queue.empty()) {
            Command cmd = state.input_queue.front();
            state.input_queue.pop();
            lock.unlock();

            if (cmd == Command::QUIT) {
                state.is_running.store(false);
                state_changed = true;
                lock.lock();
                break;
            }

            if (cmd == Command::RESTART) {
                board.reset();
                state.moves_count = 0;
                state.game_time_seconds = 0;
                last_timer_tick = std::chrono::steady_clock::now();
                state_changed = true;
            } else if (!board.get_is_game_over()) {
                bool moved = false;
                switch (cmd) {
                    case Command::UP:    moved = board.move_up(); break;
                    case Command::DOWN:  moved = board.move_down(); break;
                    case Command::LEFT:  moved = board.move_left(); break;
                    case Command::RIGHT: moved = board.move_right(); break;
                    default: break;
                }

                if (moved) {
                    state.moves_count++;
                    state_changed = true;
                }
            }

            lock.lock();
        }

        // Update elapsed game timer
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - last_timer_tick).count() >= 1) {
            if (!board.get_is_game_over() && !board.has_winner()) {
                state.game_time_seconds++;
                state_changed = true;
            }
            last_timer_tick = now;
        }

        if (state_changed) {
            {
                std::lock_guard<std::mutex> render_lock(state.render_mutex);
                state.needs_render = true;
            }
            state.render_cv.notify_one();
        }
    }

    {
        std::lock_guard<std::mutex> render_lock(state.render_mutex);
        state.needs_render = true;
    }
    state.render_cv.notify_one();
}

void Game::output_loop() {
    while (state.is_running.load()) {
        std::unique_lock<std::mutex> lock(state.render_mutex);
        state.render_cv.wait(lock, [this]() {
            return state.needs_render || !state.is_running.load();
        });

        if (!state.needs_render && !state.is_running.load()) {
            break;
        }
        state.needs_render = false;
        lock.unlock();

        // Clear terminal screen and return cursor to home
        std::cout << "\033[2J\033[H";

        // Print header
        std::cout << "========================================\n";
        std::cout << "               GAME 2048                \n";
        std::cout << "========================================\n";

        // Format game time (MM:SS)
        int minutes = state.game_time_seconds / 60;
        int seconds = state.game_time_seconds % 60;

        std::cout << " Score: " << std::setw(6) << board.get_score()
                  << " | Moves: " << std::setw(4) << state.moves_count
                  << " | Time: "
                  << std::setfill('0') << std::setw(2) << minutes << ":"
                  << std::setfill('0') << std::setw(2) << seconds
                  << std::setfill(' ') << "\n";
        std::cout << "----------------------------------------\n";

        // Print board
        printer.printBoard(std::cout);

        // Print game status
        if (board.has_winner()) {
            std::cout << "\033[92m*** YOU WIN! You reached 2048! ***\033[m\n";
        }
        if (board.get_is_game_over()) {
            std::cout << "\033[91m*** GAME OVER! No available moves left. ***\033[m\n";
        }

        std::cout << "Legend:   \033[1;92m*Green*\033[0m = newly spawned tile\n";
        std::cout << "Controls: [W/A/S/D] or Arrows: Move | [R]: Restart | [Q]: Quit\n";
        std::cout.flush();
    }

    std::cout << "\nThanks for playing 2048!\n";
}

void Game::run() {
    input_thread = std::thread(&Game::input_loop, this);
    compute_thread = std::thread(&Game::compute_loop, this);
    output_thread = std::thread(&Game::output_loop, this);

    if (input_thread.joinable()) input_thread.join();
    if (compute_thread.joinable()) compute_thread.join();
    if (output_thread.joinable()) output_thread.join();
}
