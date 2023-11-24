#include <vector>

#include "code/algebra/binary_exponentiation.hpp"
#include "code/algebra/matrix.hpp"
#include "code/utilities/random.hpp"
#include "testing.hpp"

using namespace std;

void test_pow_1() {
    testing::CURRENT_FUNC_NAME = __func__;

    const long long base = 1;
    const vector<int> exps = {0, 1, 2, 8, 16, 32};
    const vector<long long> expected_result = vector<long long>(exps.size(), 1);

    for (int i = 0; i < exps.size(); ++i) {
        const int exp = exps[i];
        const long long expected = expected_result[i];
        const long long result = pow(base, exp);

        CAPTURE_VARS(base, exp, result, expected);
        assert(result == expected);
    }
}

void test_pow_2() {
    testing::CURRENT_FUNC_NAME = __func__;

    const long long base = 2;
    const vector<int> exps = {0, 1, 2, 8, 13, 16, 31};
    const vector<long long> expected_result = {1, 2, 4, 256, 8192, 65536, 2147483648};

    for (int i = 0; i < exps.size(); ++i) {
        const int exp = exps[i];
        const long long expected = expected_result[i];
        const long long result = pow(base, exp);

        CAPTURE_VARS(base, exp, result, expected);
        assert(result == expected);
    }
}

void test_pow_others() {
    testing::CURRENT_FUNC_NAME = __func__;

    const vector<long long> bases = {13, 13, 132, 132, 34, 32, 54, 9, 3, 44};
    const vector<int> exps = {2, 9, 2, 6, 4, 5, 7, 13, 26, 3};
    const vector<long long> expected_result = {169,      10604499373,   17424,         5289852801024, 1336336,
                                               33554432, 1338925209984, 2541865828329, 2541865828329, 85184};

    for (int i = 0; i < exps.size(); ++i) {
        const long long base = bases[i];
        const int exp = exps[i];
        const long long expected = expected_result[i];
        const long long result = pow(base, exp);

        CAPTURE_VARS(base, exp, result, expected);
        assert(result == expected);
    }
}

void test_pow_matrixes() {
    testing::CURRENT_FUNC_NAME = __func__;

    const vector<matrix<long long>> bases = {{{1, 2}, {3, 4}}, {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    const vector<int> exps = {5, 6};
    const vector<matrix<long long>> expected = {
        {{1069, 1558}, {2337, 3406}},
        {{1963440, 2412504, 2861568}, {4446414, 5463369, 6480324}, {6929388, 8514234, 10099080}}
    };

    auto identity = [](const matrix<long long>& m) { return identity_matrix(m); };
    for (int i = 0; i < exps.size(); ++i) {
        assert(pow<matrix<long long>>(bases[i], exps[i], identity) == expected[i]);
    }
}

void test_pow_random_simple() {
    testing::CURRENT_FUNC_NAME = __func__;

    const int OPERATIONS = 1000;
    const int MIN_BASE = 1, MAX_BASE = 1000;
    const int MIN_EXP = 0;

    const auto pow_naive = [](long long base, long long exp) {
        long long ans = 1;
        while (exp--) ans *= base;
        return ans;
    };

    for (int i = 0; i < OPERATIONS; ++i) {
        const long long base = randint(MIN_BASE, MAX_BASE);

        const int MAX_EXP = base == 1 ? 1000 : 9 / log10(base);
        const int exp = randint(MIN_EXP, MAX_EXP);

        const int expected = pow_naive(base, exp);
        const int result = pow(base, exp);

        CAPTURE_VARS(base, exp, result, expected);
        assert(result == expected);
    }
}

void test_pow_random_mod() {
    testing::CURRENT_FUNC_NAME = __func__;

    const int OPERATIONS = 1000;
    const int MIN_BASE = 1, MAX_BASE = 1000;
    const int MIN_MOD = 1007, MAX_MOD = 1000000007;
    const int MIN_EXP = 0;

    const auto pow_naive = [](long long base, long long exp, long long mod) {
        long long ans = 1;
        while (exp--) ans = (ans * base) % mod;
        return ans;
    };

    for (int i = 0; i < OPERATIONS; ++i) {
        const long long base = randint(MIN_BASE, MAX_BASE);

        const int MAX_EXP = base == 1 ? 1000 : 9 / log10(base);
        const int exp = randint(MIN_EXP, MAX_EXP);

        const long long mod = randint(MIN_MOD, MAX_MOD);

        const int expected = pow_naive(base, exp, mod);
        const int result = pow(base, exp, mod);

        CAPTURE_VARS(base, exp, mod, result, expected);
        assert(result == expected);
    }
}

int main() {
    return testing::run_tests(
        {test_pow_1, test_pow_2, test_pow_others, test_pow_matrixes, test_pow_random_simple, test_pow_random_mod}
    );
}
