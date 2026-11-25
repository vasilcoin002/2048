//
// Created by Vas Klu on 18.10.2026.
//

#include "Utils.h"
#include <random>

static std::mt19937& get_rng() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

void Utils::set_seed(unsigned int seed) {
    get_rng().seed(seed);
}

int Utils::randint(int from, int to) {
    std::uniform_int_distribution<int> distrib(from, to);

    return distrib(get_rng());
}
