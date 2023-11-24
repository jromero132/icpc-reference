#include <stdbool.h>

#include <cmath>
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

        CAPTURE_VARS(n, m, a);
        assert(a.size() == n && a[0].size() == m);
        CAPTURE_VARS(n, m, a);

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

            CAPTURE_VARS(n, a);
            assert(a.size() == n && a[0].size() == n);
            CAPTURE_VARS(n, a);

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

    matrix<int> expected_matrix = {{10}};

    matrix<int> m_result = m1 + m2 + m3 + m4;

    CAPTURE_VARS(m1, m2, m3, m4, m_result, expected_matrix);
    assert(m_result == expected_matrix);
}

void test_sum_matrix_2() {
    testing::CURRENT_FUNC_NAME = __func__;

    matrix<int> m1 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    matrix<int> m2 = {{13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24}};
    matrix<int> m3 = {
        {375207, 778631, 773010, 877366}, {758033, 677918, 728003, 232431}, {323897, 583497, 710524, 622414}
    };

    matrix<int> expected_matrix = {
        {375221, 778647, 773028, 877386}, {758055, 677942, 728029, 232459}, {323927, 583529, 710558, 622450}
    };

    matrix<int> m_result = m1 + m2;
    m_result = m3 + m_result;

    CAPTURE_VARS(m1, m2, m3, m_result, expected_matrix);
    assert(m_result == expected_matrix);
}

void test_sub_matrix_1() {
    testing::CURRENT_FUNC_NAME = __func__;

    matrix<int> m1 = {{1}};
    matrix<int> m2 = {{2}};
    matrix<int> m3 = {{3}};
    matrix<int> m4 = {{4}};

    matrix<int> expected_matrix = {{4}};

    matrix<int> m_result = m1 + m2 - m3 + m4;

    CAPTURE_VARS(m1, m2, m3, m4, m_result, expected_matrix);
    assert(m_result == expected_matrix);
}

void test_sub_matrix_2() {
    testing::CURRENT_FUNC_NAME = __func__;

    matrix<int> m1 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    matrix<int> m2 = {{13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24}};
    matrix<int> m3 = {
        {375207, 778631, 773010, 877366}, {758033, 677918, 728003, 232431}, {323897, 583497, 710524, 622414}
    };

    matrix<int> expected_matrix = {
        {375193, 778615, 772992, 877346}, {758011, 677894, 727977, 232403}, {323867, 583465, 710490, 622378}
    };

    matrix<int> m_result = m1 + m2;
    m_result = m3 - m_result;

    CAPTURE_VARS(m1, m2, m3, m_result, expected_matrix);
    assert(m_result == expected_matrix);
}

void test_mul_matrixes_1() {
    testing::CURRENT_FUNC_NAME = __func__;

    matrix<int> m1 = {{1}};
    matrix<int> m2 = {{1}};
    matrix<int> m3 = {{1}};

    matrix<int> expected_matrix = {{1}};

    matrix<int> m_result = m1 * m2 * m3;

    CAPTURE_VARS(m1, m2, m3, m_result, expected_matrix);
    assert(m_result == expected_matrix);
}

void test_mul_matrixes_2() {
    testing::CURRENT_FUNC_NAME = __func__;

    matrix<int> m1 = {{1, 2, 3}, {4, 5, 6}};
    matrix<int> m2 = {{13, 132}, {56, 99}, {84, 48}};
    matrix<int> m3 = {{101, 25}, {8, 32}};

    matrix<int> expected_matrix = {{41869, 24593}, {94924, 62852}};

    matrix<int> m_result = m1 * m2 * m3;

    CAPTURE_VARS(m1, m2, m3, m_result, expected_matrix);
    assert(m_result == expected_matrix);
}

void test_transpose() {
    testing::CURRENT_FUNC_NAME = __func__;

    vector<matrix<int>> matrixes = {{{1}}, {{1, 2}, {3, 4}}, {{13, 132}}, {{13}, {132}}};
    vector<matrix<int>> expected_matrixes = {{{1}}, {{1, 3}, {2, 4}}, {{13}, {132}}, {{13, 132}}};

    for (int i = 0; i < matrixes.size(); ++i) {
        matrix<int> m = matrixes[i];
        matrix<int> m_transpose = transpose(m);
        matrix<int> expected_matrix = expected_matrixes[i];

        CAPTURE_VARS(m, m_transpose, expected_matrix);
        assert(m_transpose == expected_matrix);
    }
}

void test_determinant_and_gauss() {
    testing::CURRENT_FUNC_NAME = __func__;

    vector<matrix<int>> matrixes = {
        {{1, 1}, {1, 1}},
        {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}},
        {{1, 2}, {3, 4}},
        {{13, 132, 2}, {32, 25, 169}, {8, 12, 156}}
    };
    vector<vector<int>> solutions = {{}, {}, {1, 1}, {1, 3, 2}};

    vector<double> expected_determinants = {0, 0, -2, -455776};
    vector<vector<double>> expected_solutions = {{}, {}, {-1, 1}, {0.0336042266376, 0.0041028926490, 0.0107816120199}};

    for (int i = 0; i < matrixes.size(); ++i) {
        matrix<int> m = matrixes[i];
        vector<int> solution = solutions[i];
        double expected_determinant = expected_determinants[i];
        vector<double> expected_solution = expected_solutions[i];

        pair<double, vector<double>> tmp = determinant_and_gauss(m, solution);
        double det = tmp.first;
        vector<double> sol = tmp.second;

        CAPTURE_VARS(m, solution, det, expected_determinant, MATRIX_EPS, sol, expected_solution);
        assert(fabs(det - expected_determinant) < MATRIX_EPS && sol.size() == expected_solution.size());
        CAPTURE_VARS(m, solution, det, expected_determinant, MATRIX_EPS, sol, expected_solution);

        for (int j = 0; j < sol.size(); ++j) {
            assert(fabs(sol[j] - expected_solutions[i][j]) < MATRIX_EPS);
        }
    }
}

int main() {
    return testing::run_tests(
        {test_null_matrix, test_identity_matrix, test_sum_matrix_1, test_sum_matrix_2, test_sub_matrix_1,
         test_sub_matrix_2, test_mul_matrixes_1, test_mul_matrixes_2, test_transpose, test_determinant_and_gauss}
    );
}
