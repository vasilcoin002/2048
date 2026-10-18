//
// Created by Vas Klu on 18.10.2026.
//

#include "Utils.h"

#include <random>

int Utils::get_random_number(int from, int to) {
    std::random_device rd;

    // 2. Ініціалізуємо генератор Mersenne Twister цим зерном
    std::mt19937 gen(rd());

    // 3. Визначаємо діапазон (у цьому прикладі від 1 до 100 включно)
    std::uniform_int_distribution<int> distrib(1, 100);

    // 4. Отримуємо випадкове число
    int random_number = distrib(gen);
}
