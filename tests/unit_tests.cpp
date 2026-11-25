#include <iostream>
#include <cassert>
#include <vector>
#include "Board.h"
#include "ArgParser.h"
#include "Utils.h"

// Simple testing macros
static int tests_passed = 0;
static int tests_total = 0;

#define RUN_TEST(test_func) \
    do { \
        tests_total++; \
        std::cout << "[RUN]  " << #test_func << "... "; \
        test_func(); \
        tests_passed++; \
        std::cout << "\033[92mPASSED\033[0m\n"; \
    } while(0)

// Test 1: Board initialization
void test_board_init() {
    Board board(4, 4, 2048);
    assert(board.get_width() == 4);
    assert(board.get_height() == 4);
    assert(board.get_target() == 2048);
    assert(board.get_score() == 0);
    assert(!board.get_is_game_over());
    assert(!board.has_winner());
    assert(!board.can_undo());

    board.generate_board();

    // Exactly 2 tiles must be spawned at start
    int count = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int val = board.get_tile(i, j);
            if (val != 0) {
                assert(val == 2 || val == 4);
                count++;
            }
        }
    }
    assert(count == 2);
}

// Test 2: Moving tiles without merge
void test_tile_movement() {
    Board board(4, 4, 2048);
    board.set_tile(2, {0, 2});
    board.set_tile(4, {1, 3});

    // Move left
    bool moved = board.move_left();
    assert(moved);
    assert(board.get_tile(0, 0) == 2);
    assert(board.get_tile(1, 0) == 4);
}

// Test 3: Merging identical tiles (2 + 2 = 4)
void test_tile_merging() {
    Board board(4, 4, 2048);
    board.set_tile(2, {0, 0});
    board.set_tile(2, {0, 1});

    bool moved = board.move_left();
    assert(moved);
    assert(board.get_tile(0, 0) == 4);
    assert(board.get_score() == 4);
}

// Test 4: Prevent double-merging in a single move: [2, 2, 2, 2] -> [4, 4, 0, 0], NOT [8, 0, 0, 0]
void test_no_double_merge() {
    Board board(4, 4, 2048);
    board.set_tile(2, {0, 0});
    board.set_tile(2, {0, 1});
    board.set_tile(2, {0, 2});
    board.set_tile(2, {0, 3});

    bool moved = board.move_left();
    assert(moved);
    assert(board.get_tile(0, 0) == 4);
    assert(board.get_tile(0, 1) == 4);
    assert(board.get_score() == 8);
}

// Test 5: Win condition detection
void test_win_condition() {
    Board board(4, 4, 512);
    assert(!board.has_winner());

    board.set_tile(256, {0, 0});
    board.set_tile(256, {0, 1});
    board.move_left();

    assert(board.get_tile(0, 0) == 512);
    assert(board.has_winner());
}

// Test 6: Game Over condition (full board with no possible merges)
void test_game_over() {
    Board board(3, 3, 2048);
    // Fill board with alternating numbers
    // 2 4 2
    // 4 2 4
    // 2 4 2
    int val = 2;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board.set_tile(((i + j) % 2 == 0) ? 2 : 4, {i, j});
        }
    }

    // No moves can be made
    assert(board.check_game_over());
    assert(board.get_is_game_over());
    assert(!board.move_up());
    assert(!board.move_down());
    assert(!board.move_left());
    assert(!board.move_right());
}

// Test 7: Undo functionality
void test_undo_functionality() {
    Board board(4, 4, 2048);
    board.set_tile(2, {0, 0});
    board.set_tile(2, {0, 1});

    assert(!board.can_undo());

    board.move_left();
    assert(board.get_score() == 4);
    assert(board.can_undo());

    // Execute undo
    bool undone = board.undo();
    assert(undone);
    assert(!board.can_undo());
    assert(board.get_tile(0, 0) == 2);
    assert(board.get_tile(0, 1) == 2);
    assert(board.get_score() == 0);
}

// Test 8: ArgParser valid arguments
void test_argparser_valid() {
    char prog[] = "2048";
    char flag_s[] = "-s";
    char val_s[] = "5";
    char flag_t[] = "--target";
    char val_t[] = "1024";
    char flag_seed[] = "--seed";
    char val_seed[] = "12345";

    char* argv[] = {prog, flag_s, val_s, flag_t, val_t, flag_seed, val_seed};
    int argc = 7;

    ArgParser parser(argc, argv);
    assert(!parser.has_error());
    assert(!parser.is_help_requested());
    assert(parser.get_size() == 5);
    assert(parser.get_target() == 1024);
    assert(parser.has_seed());
    assert(parser.get_seed() == 12345);
}

// Test 9: ArgParser invalid arguments validation
void test_argparser_invalid() {
    // Non-power of 2 target
    {
        char prog[] = "2048";
        char flag_t[] = "-t";
        char val_t[] = "15";
        char* argv[] = {prog, flag_t, val_t};
        ArgParser parser(3, argv);
        assert(parser.has_error());
    }

    // Size too small (< 3)
    {
        char prog[] = "2048";
        char flag_s[] = "--size";
        char val_s[] = "2";
        char* argv[] = {prog, flag_s, val_s};
        ArgParser parser(3, argv);
        assert(parser.has_error());
    }

    // Unrecognized option
    {
        char prog[] = "2048";
        char flag_unknown[] = "--invalid-flag";
        char* argv[] = {prog, flag_unknown};
        ArgParser parser(2, argv);
        assert(parser.has_error());
    }
}

int main() {
    std::cout << "========================================\n";
    std::cout << "       RUNNING 2048 UNIT TESTS         \n";
    std::cout << "========================================\n";

    RUN_TEST(test_board_init);
    RUN_TEST(test_tile_movement);
    RUN_TEST(test_tile_merging);
    RUN_TEST(test_no_double_merge);
    RUN_TEST(test_win_condition);
    RUN_TEST(test_game_over);
    RUN_TEST(test_undo_functionality);
    RUN_TEST(test_argparser_valid);
    RUN_TEST(test_argparser_invalid);

    std::cout << "========================================\n";
    std::cout << "Results: " << tests_passed << " / " << tests_total << " tests passed.\n";
    std::cout << "========================================\n";

    return (tests_passed == tests_total) ? 0 : 1;
}
