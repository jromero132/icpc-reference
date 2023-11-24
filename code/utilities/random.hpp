#include <chrono>
#include <random>

std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

template <typename IntType = int>
static IntType randint(IntType lo, IntType hi) {
    // A random number in the interval [a, b]

    return std::uniform_int_distribution<IntType>(lo, hi)(rng);
}