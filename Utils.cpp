//
// Created by Vas Klu on 18.10.2026.
//

#include "Utils.h"

#include <random>

int Utils::randint(int from, int to) {
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> distrib(from, to);

    return distrib(gen);
}
