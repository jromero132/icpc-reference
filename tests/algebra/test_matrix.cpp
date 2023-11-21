#include <stdbool.h>

#include <cassert>
#include <vector>

#include "code/algebra/matrix.hpp"
#include "testing.hpp"

using namespace std;

template <typename T>
bool operator==(const matrix<T>& a, const matrix<T>& b) {
    int n = a.size(), m = a[0].size();
    if (n != b.size() || m != b[0].size()) {
        return false;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] != b[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
bool operator!=(const matrix<T>& a, const matrix<T>& b) {
    return !(a == b);
}

void test_null_matrix() {
    testing::CURRENT_FUNC_NAME = __func__;

    vector<vector<int>> dims = {{1, 1}, {1, 13}, {13, 1}, {2, 13}, {13, 2}, {132, 132}, {13, 132}, {132, 13}};
    for (auto dim : dims) {
        int n = dim[0], m = dim[1];
        matrix<int> a = null_matrix<int>(n, m);
        assert(a.size() == n && a[0].size() == m);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                assert(a[i][j] == 0);
            }
        }
    }
}

void test_identity_matrix() {
    testing::CURRENT_FUNC_NAME = __func__;

    vector<int> dims = {1, 2, 13, 132};
    for (int n : dims) {
        matrix<int> a;
        for (int k = 0; k < 2; ++k) {
            if (k == 0) {
                a = identity_matrix<int>(n);
            }
            else {
                a = identity_matrix<int>(a);
            }

            assert(a.size() == n && a[0].size() == n);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    assert(a[i][j] == (i == j));
                }
            }
        }
    }
}

void test_sum_matrix_1() {
    testing::CURRENT_FUNC_NAME = __func__;

    matrix<int> m1 = {{1}};
    matrix<int> m2 = {{2}};
    matrix<int> m3 = {{3}};
    matrix<int> m4 = {{4}};

    int expected_n = 1, expected_m = 1;
    matrix<int> expected_matrix = {{10}};

    matrix<int> m_result = m1 + m2 + m3 + m4;
    assert(m_result.size() == expected_n && m_result[0].size() == expected_m && m_result == expected_matrix);
}

void test_sum_matrix_2() {
    testing::CURRENT_FUNC_NAME = __func__;

    matrix<int> m1 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    matrix<int> m2 = {{13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24}};
    matrix<int> m3 = {
        {375207, 778631, 773010, 877366}, {758033, 677918, 728003, 232431}, {323897, 583497, 710524, 622414}
    };

    int expected_n = 3, expected_m = 4;
    matrix<int> expected_matrix = {
        {375221, 778647, 773028, 877386}, {758055, 677942, 728029, 232459}, {323927, 583529, 710558, 622450}
    };

    matrix<int> m_result = m1 + m2;
    m_result = m3 + m_result;
    assert(m_result.size() == expected_n && m_result[0].size() == expected_m && m_result == expected_matrix);
}

void test_sub_matrix_1() {
    testing::CURRENT_FUNC_NAME = __func__;

    matrix<int> m1 = {{1}};
    matrix<int> m2 = {{2}};
    matrix<int> m3 = {{3}};
    matrix<int> m4 = {{4}};

    int expected_n = 1, expected_m = 1;
    matrix<int> expected_matrix = {{4}};

    matrix<int> m_result = m1 + m2 - m3 + m4;
    assert(m_result.size() == expected_n && m_result[0].size() == expected_m && m_result == expected_matrix);
}

void test_sub_matrix_2() {
    testing::CURRENT_FUNC_NAME = __func__;

    matrix<int> m1 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    matrix<int> m2 = {{13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24}};
    matrix<int> m3 = {
        {375207, 778631, 773010, 877366}, {758033, 677918, 728003, 232431}, {323897, 583497, 710524, 622414}
    };

    int expected_n = 3, expected_m = 4;
    matrix<int> expected_matrix = {
        {375193, 778615, 772992, 877346}, {758011, 677894, 727977, 232403}, {323867, 583465, 710490, 622378}
    };

    matrix<int> m_result = m1 + m2;
    m_result = m3 - m_result;
    assert(m_result.size() == expected_n && m_result[0].size() == expected_m && m_result == expected_matrix);
}

void test_mul_matrixes_1() {
    testing::CURRENT_FUNC_NAME = __func__;

    matrix<int> m1 = {{1}};
    matrix<int> m2 = {{1}};
    matrix<int> m3 = {{1}};

    int expected_n = 1, expected_m = 1;
    matrix<int> expected_matrix = {{1}};

    matrix<int> m_result = m1 * m2 * m3;
    assert(m_result.size() == expected_n && m_result[0].size() == expected_m && m_result == expected_matrix);
}

void test_mul_matrixes_2() {
    testing::CURRENT_FUNC_NAME = __func__;

    matrix<int> m1 = {{1, 2, 3}, {4, 5, 6}};
    matrix<int> m2 = {{13, 132}, {56, 99}, {84, 48}};
    matrix<int> m3 = {{101, 25}, {8, 32}};

    int expected_n = 2, expected_m = 2;
    matrix<int> expected_matrix = {{41869, 24593}, {94924, 62852}};

    matrix<int> m_result = m1 * m2 * m3;
    assert(m_result.size() == expected_n && m_result[0].size() == expected_m && m_result == expected_matrix);
}

void test_transpose() {
    testing::CURRENT_FUNC_NAME = __func__;

    vector<matrix<int>> matrixes = {{{1}}, {{1, 2}, {3, 4}}, {{13, 132}}, {{13}, {132}}};
    vector<vector<int>> expected_dim = {{1, 1}, {2, 2}, {2, 1}, {1, 2}};
    vector<matrix<int>> expected_matrixes = {{{1}}, {{1, 3}, {2, 4}}, {{13}, {132}}, {{13, 132}}};

    for (int i = 0; i < matrixes.size(); ++i) {
        matrix<int> m_transpose = transpose(matrixes[i]);

        int n = expected_dim[i][0], m = expected_dim[i][1];
        matrix<int> expected_matrix = expected_matrixes[i];

        assert(m_transpose.size() == n && m_transpose[0].size() == m && m_transpose == expected_matrix);
    }
}

int main() {
    return testing::run_tests(
        {test_null_matrix, test_identity_matrix, test_sum_matrix_1, test_sum_matrix_2, test_sub_matrix_1,
         test_sub_matrix_2, test_mul_matrixes_1, test_mul_matrixes_2, test_transpose}
    );
}
