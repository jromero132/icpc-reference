#include <cassert>
#include <vector>

template <typename T>
using matrix_row = std::vector<T>;

template <typename T>
using matrix = std::vector<matrix_row<T>>;

template <typename T>
matrix<T> null_matrix(const int n, const int m) {
    return matrix<T>(n, matrix_row<T>(m, (T)0));
}

template <typename T>
matrix<T> identity_matrix(const int n) {
    matrix<T> ans = matrix<T>(n, matrix_row<T>(n, (T)0));
    for (int i = 0; i < n; ++i) {
        ans[i][i] = (T)1;
    }
    return ans;
}

template <typename T>
matrix<T> identity_matrix(const matrix<T>& m) {
    assert(m.size() == m[0].size());
    return identity_matrix<T>(m.size());
}

template <typename T>
matrix<T> operator+(const matrix<T>& a, const matrix<T>& b) {
    int n = a.size(), m = a[0].size();
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
    int n = a.size(), m = a[0].size();
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
    int n = a.size(), m = a[0].size(), o = b[0].size();
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
    matrix<T> ans;
    int n = a.size(), m = a[0].size();
    for (int i = 0; i < m; ++i) {
        ans.push_back(matrix_row<T>(n));
        for (int j = 0; j < n; ++j) {
            ans[i][j] = a[j][i];
        }
    }
    return ans;
}