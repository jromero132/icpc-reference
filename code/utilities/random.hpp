#include <chrono>
#include <random>

std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

static int randint(int lo, int hi) {
    // A random number in the interval [a, b]

    return std::uniform_int_distribution<int>(lo, hi)(rng);
}