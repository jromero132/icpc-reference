#include <vector>

#include "code/utilities/random.hpp"
#include "testing.hpp"

using namespace std;

void test_monobit() {
    testing::CURRENT_FUNC_NAME = __func__;

    // Test whether the number of zeroes and ones ones in a sequence are approximately the same as would be expected
    // for a truly random sequence. The test assesses the closeness of the fraction of ones to 1/2, that is, the
    // number of ones and zeroes in a sequence should be about the same.

    const double expected = 0.01;  // Decision rule: 1% level. Considered non-random if smaller.
    const int N = (int)1e7;        // Amount of numbers to generate

    int Sn = 0;  // Sn = sum(Xi), 0 <= i < N
    for (int i = 0; i < N; ++i) {
        Sn += 2 * randint(0, 1) - 1;  // Xi = 2 * rand - 1 (basically -1 for 0 and 1 for 1)
    }

    const double Sobs = abs(Sn) / sqrt(N);       // Sobs = |Sn| / sqrt(N)
    const double Pvalue = erfc(Sobs / sqrt(2));  // Pvalue = erfc(Sobs / sqrt(2))
    CAPTURE_VARS(N, Sn, Sobs, Pvalue, expected);
    assert(Pvalue >= expected);
}

void test_interval() {
    testing::CURRENT_FUNC_NAME = __func__;

    const int degrees_of_freedom = 100;
    const int N = (int)1e7;  // amount of numbers to generate
    const int initial_value = randint(0, (int)1e9);
    const double expected_frequency = (double)N / (degrees_of_freedom + 1);
    const double expected_chi_square = 135.807;

    std::vector<int> results(degrees_of_freedom + 1);
    for (int i = 0; i < N; ++i) {
        ++results[randint(initial_value, initial_value + degrees_of_freedom) - initial_value];
    }

    double chi_square = 0;
    for (int i = 0; i <= degrees_of_freedom; ++i) {
        const double diff = results[i] - expected_frequency;
        chi_square += diff * diff / expected_frequency;
    }

    CAPTURE_VARS(degrees_of_freedom, N, initial_value, results, expected_frequency, chi_square, expected_chi_square);
    assert(chi_square <= expected_chi_square);
}

void test_expected_number() {
    testing::CURRENT_FUNC_NAME = __func__;

    const int N = (int)1e7;
    const int range = (int)1e6;
    const int limit = (int)1e9;
    const double confidence = 0.99;

    const int range_left = randint(0, limit + 1);
    const int range_right = range_left + range;
    const double expected = (range_left + range_right) / 2.0;

    double sum = 0;
    for (int i = 0; i < N; ++i) sum += randint(range_left, range_right);

    const double result = sum / N;

    CAPTURE_VARS(N, limit, confidence, range, range_left, range_right, sum, result, expected);
    assert(expected * confidence <= result && result <= expected * (2 - confidence));
}

void test_average_distance() {
    testing::CURRENT_FUNC_NAME = __func__;

    const int N = (int)3e6;
    const double expected = N / 3.0;
    const double confidence = 0.99;

    std::vector<int> permutation(N);
    for (int i = 0; i < N; ++i) permutation[i] = i;

    for (int i = 1; i < N; i++) std::swap(permutation[i], permutation[randint(0, i)]);

    double distance_sum = 0;
    for (int i = 0; i < N; ++i) distance_sum += abs(permutation[i] - i);
    distance_sum /= N;

    CAPTURE_VARS(N, permutation, confidence, distance_sum, expected);
    assert(expected * confidence <= distance_sum && distance_sum <= expected * (2 - confidence));
}

int main() {
    return testing::run_tests(
        {testing::at_least_n_of_m(test_monobit, 5, 2), testing::at_least_n_of_m(test_interval, 5),
         testing::at_least_n_of_m(test_expected_number, 5), testing::at_least_n_of_m(test_average_distance, 5)}
    );
}
