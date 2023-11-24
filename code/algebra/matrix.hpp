#include <cassert>
#include <cmath>
#include <vector>

const double MATRIX_EPS = 1e-9;

template <typename T>
using matrix_row = std::vector<T>;

template <typename T>
using matrix = std::vector<matrix_row<T>>;

template <typename T>
matrix<T> null_matrix(const int n, const int m) {
    // Time complexity: O(n * m * T1)
    // Memory complexity: O(n * m * M1)
    //   - n: number of rows
    //   - m: number of columns
    //   - T1: time complexity of creating one T value
    //   - M1: memory complexity of creating one T value
    // e.g. using integer or double values: O(n * m) ; O(n * m)

    return matrix<T>(n, matrix_row<T>(m, (T)0));
}

template <typename T>
matrix<T> identity_matrix(const int n) {
    // Time complexity: O(n^2 * T1)
    // Memory complexity: O(n^2 * M1)
    //   - n: number of rows and columns
    //   - T1: time complexity of creating one T value
    //   - M1: memory complexity of creating one T value
    // e.g. using integer or double values: O(n^2) ; O(n^2)

    matrix<T> ans = matrix<T>(n, matrix_row<T>(n, (T)0));
    for (int i = 0; i < n; ++i) {
        ans[i][i] = (T)1;
    }
    return ans;
}

template <typename T>
matrix<T> identity_matrix(const matrix<T>& m) {
    // Time complexity: O(n^2 * T1)
    // Memory complexity: O(n^2 * M1)
    //   - n: number of rows and columns
    //   - T1: time complexity of creating one T value
    //   - M1: memory complexity of creating one T value
    // e.g. using integer or double values: O(n^2) ; O(n^2)

    assert(m.size() == m[0].size());
    return identity_matrix<T>(m.size());
}

template <typename T>
matrix<T> operator+(const matrix<T>& a, const matrix<T>& b) {
    // Time complexity: O(n * m * T1)
    // Memory complexity: O(n * m * M1)
    //   - n: number of rows
    //   - m: number of columns
    //   - T1: time complexity of adding and assigning two T values
    //   - M1: memory complexity of creating one T value
    // e.g. using integer or double values: O(n * m) ; O(n * m)

    const int n = a.size(), m = a[0].size();
    assert(n == b.size() && m == b[0].size());

    matrix<T> ans;
    for (int i = 0; i < n; ++i) {
        ans.push_back(matrix_row<T>(m, (T)0));
        for (int j = 0; j < m; ++j) {
            ans[i][j] = a[i][j] + b[i][j];
        }
    }
    return ans;
}

template <typename T>
matrix<T> operator-(const matrix<T>& a, const matrix<T>& b) {
    // Time complexity: O(n * m * T1)
    // Memory complexity: O(n * m * M1)
    //   - n: number of rows
    //   - m: number of columns
    //   - T1: time complexity of subtracting and assigning two T values
    //   - M1: memory complexity of creating one T value
    // e.g. using integer or double values: O(n * m) ; O(n * m)

    const int n = a.size(), m = a[0].size();
    assert(n == b.size() && m == b[0].size());

    matrix<T> ans;
    for (int i = 0; i < n; ++i) {
        ans.push_back(matrix_row<T>(m, (T)0));
        for (int j = 0; j < m; ++j) {
            ans[i][j] = a[i][j] - b[i][j];
        }
    }
    return ans;
}

template <typename T>
matrix<T> operator*(const matrix<T>& a, const matrix<T>& b) {
    // Time complexity: O(n * m * o * T1)
    // Memory complexity: O(n * o * M1)
    //   - n: number of rows of a
    //   - m: number of columns of a (same as number of rows of b)
    //   - o: number of columns of b
    //   - T1: time complexity of multiplying, adding and assigning two T values
    //   - M1: memory complexity of creating one T value
    // e.g. using integer or double values: O(n * m * o) ; O(n * o)

    const int n = a.size(), m = a[0].size(), o = b[0].size();
    assert(m == b.size());

    matrix<T> ans;
    for (int i = 0; i < n; ++i) {
        ans.push_back(matrix_row<T>(o, (T)0));
        for (int k = 0; k < m; ++k) {
            for (int j = 0; j < o; ++j) {
                ans[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return ans;
}

template <typename T>
matrix<T> transpose(const matrix<T>& a) {
    // Time complexity: O(n * m * T1)
    // Memory complexity: O(n * m * M1)
    //   - n: number of rows
    //   - m: number of columns
    //   - T1: time complexity of assigning one T value
    //   - M1: memory complexity of creating one T value
    // e.g. using integer or double values: O(n * m) ; O(n * m)

    const int n = a.size(), m = a[0].size();

    matrix<T> ans;
    for (int i = 0; i < m; ++i) {
        ans.push_back(matrix_row<T>(n));
        for (int j = 0; j < n; ++j) {
            ans[i][j] = a[j][i];
        }
    }
    return ans;
}

template <typename T>
std::pair<double, std::vector<double>> determinant_and_gauss(
    const matrix<T>& a, const std::vector<T>& result = std::vector<T>()
) {
    // Time complexity: O(n^3 * T1)
    // Memory complexity: O(n^2 * M1)
    //   - n: number of rows and columns of a
    //   - T1: time complexity of subtract, multiply, dividing and assigning two T values
    //   - M1: memory complexity of creating one T value
    // e.g. using integer or double values: O(n^3) ; O(n^2)

    const int n = a.size();
    assert(n == a[0].size() && (result.size() == 0 || result.size() == n));

    std::vector<double> sol(n);
    matrix<double> mat(n, matrix_row<double>(n));
    for (int i = 0; i < n; ++i) {
        if (result.size() != 0) {
            sol[i] = result[i];
        }
        for (int j = 0; j < n; ++j) {
            mat[i][j] = a[i][j];
        }
    }

    double det = 1;
    for (int i = 0, p = 0; i < n; p = ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (fabs(mat[p][i]) < fabs(mat[j][i])) {
                p = j;
            }
        }
        std::swap(mat[i], mat[p]);
        std::swap(sol[i], sol[p]);
        if (fabs(mat[i][i]) < MATRIX_EPS) {
            return {0, {}};
        }
        for (int j = i + 1; j < n; ++j) {
            for (int k = i + 1; k < n; ++k) {
                mat[j][k] -= mat[i][k] * mat[j][i] / mat[i][i];
            }
            sol[j] -= sol[i] * mat[j][i] / mat[i][i];
        }
        det *= mat[i][i];
        if (p != i) {
            det = -det;
        }
    }
    for (int i = n - 1; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            sol[i] -= mat[i][j] * sol[j];
        }
        sol[i] /= mat[i][i];
    }
    return {det, sol};
}