#include <cassert>
#include <vector>

#include "code/algebra/binary_exponentiation.hpp"
#include "testing.hpp"

using namespace std;

void test_pow_1() {
    testing::CURRENT_FUNC_NAME = __func__;

    long long base = 1;
    vector<int> exps = {0, 1, 2, 8, 16, 32};
    vector<long long> expected = vector<long long>(exps.size(), 1);

    for (int i = 0; i < exps.size(); ++i) {
        assert(pow(base, exps[i]) == expected[i]);
    }
}

void test_pow_2() {
    testing::CURRENT_FUNC_NAME = __func__;

    long long base = 2;
    vector<int> exps = {0, 1, 2, 8, 13, 16, 31};
    vector<long long> expected = {1, 2, 4, 256, 8192, 65536, 2147483648};

    for (int i = 0; i < exps.size(); ++i) {
        assert(pow(base, exps[i]) == expected[i]);
    }
}

void test_pow_others() {
    testing::CURRENT_FUNC_NAME = __func__;

    vector<long long> bases = {13, 13, 132, 132, 34, 32, 54, 9, 3, 44};
    vector<int> exps = {2, 9, 2, 6, 4, 5, 7, 13, 26, 3};
    vector<long long> expected = {169,      10604499373,   17424,         5289852801024, 1336336,
                                  33554432, 1338925209984, 2541865828329, 2541865828329, 85184};

    for (int i = 0; i < exps.size(); ++i) {
        assert(pow(bases[i], exps[i]) == expected[i]);
    }
}

#include "code/algebra/matrix.hpp"

void test_pow_matrixes() {
    testing::CURRENT_FUNC_NAME = __func__;

    vector<matrix<long long>> bases = {{{1, 2}, {3, 4}}, {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    vector<int> exps = {5, 6};
    vector<matrix<long long>> expected = {
        {{1069, 1558}, {2337, 3406}},
        {{1963440, 2412504, 2861568}, {4446414, 5463369, 6480324}, {6929388, 8514234, 10099080}}
    };

    auto identity = [](const matrix<long long>& m) { return identity_matrix(m); };
    for (int i = 0; i < exps.size(); ++i) {
        assert(pow<matrix<long long>>(bases[i], exps[i], identity) == expected[i]);
    }
}

int main() { return testing::run_tests({test_pow_1, test_pow_2, test_pow_others, test_pow_matrixes}); }
