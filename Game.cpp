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
    set_raw_mode(false);
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

void Game::render() {
    // Clear terminal screen and return cursor to home
    std::cout << "\033[2J\033[H";

    // Print header
    std::cout << "========================================\n";
    std::cout << "               GAME 2048                \n";
    std::cout << "========================================\n";

    std::cout << " Score: " << std::setw(6) << board.get_score()
              << " | Moves: " << std::setw(4) << moves_count << "\n";
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

void Game::process_input() {
    char ch = 0;
    ssize_t bytes_read = read(STDIN_FILENO, &ch, 1);

    if (bytes_read <= 0) {
        is_running = false;
        return;
    }

    bool moved = false;

    if (ch == 'q' || ch == 'Q') {
        is_running = false;
        return;
    } else if (ch == 'r' || ch == 'R') {
        board.reset();
        moves_count = 0;
        return;
    } else if (ch == 'w' || ch == 'W') {
        if (!board.get_is_game_over()) moved = board.move_up();
    } else if (ch == 's' || ch == 'S') {
        if (!board.get_is_game_over()) moved = board.move_down();
    } else if (ch == 'a' || ch == 'A') {
        if (!board.get_is_game_over()) moved = board.move_left();
    } else if (ch == 'd' || ch == 'D') {
        if (!board.get_is_game_over()) moved = board.move_right();
    } else if (ch == '\033') { // Escape sequence (e.g. arrow keys)
        char seq[2] = {0, 0};
        if (read(STDIN_FILENO, &seq[0], 1) > 0 && seq[0] == '[') {
            if (read(STDIN_FILENO, &seq[1], 1) > 0) {
                if (!board.get_is_game_over()) {
                    switch (seq[1]) {
                        case 'A': moved = board.move_up(); break;
                        case 'B': moved = board.move_down(); break;
                        case 'C': moved = board.move_right(); break;
                        case 'D': moved = board.move_left(); break;
                        default: break;
                    }
                }
            }
        } else {
            is_running = false; // Solo Esc quits game
            return;
        }
    }

    if (moved) {
        moves_count++;
    }
}

void Game::run() {
    set_raw_mode(true);

    while (is_running) {
        render();
        process_input();
    }

    set_raw_mode(false);
    std::cout << "\nThanks for playing 2048!\n";
}
