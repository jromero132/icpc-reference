#include <chrono>
#include <random>

std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

static int randint(int lo, int hi) {
    return std::uniform_int_distribution<int>(lo, hi)(rng);
}