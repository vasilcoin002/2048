//
// Created by Vas Klu on 18.10.2026.
//

#include "Utils.h"

#include <random>

int Utils::randint(int from, int to) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(from, to);

    int random_number = distrib(gen);

    return random_number;
}
