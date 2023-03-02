#include "code/data_structures/fenwick_tree.hpp"

#include <tuple>
#include <vector>

#include "catch2/catch.hpp"

template <typename T>
fenwick<T> build_fenwick_tree(std::vector<T> v) {
    int N = (int)v.size();
    fenwick<T> f(N);
    for(int i = 0; i < N; ++i)
        f.update(i + 1, v[i]);
    return f;
}

TEST_CASE("SumFenwickTree", "[fenwick_tree]") {
    // Calculate the sum of a all elements in a range.
    // Range is [a, b) and 0-index.

    std::vector<int> a = {1, 10, 100, 1000, 10000};
    std::vector<std::pair<int, int>> queries = {{2, 3}, {0, 3}, {2, 5}, {3, 4}, {0, 5}};
    std::vector<int> expected = {100, 111, 11100, 1000, 11111};

    fenwick<int> f = build_fenwick_tree(a);
    int Q = (int)queries.size();
    for(int i = 0; i < Q; ++i) {
        int left = queries[i].first;
        int right = queries[i].second;
        int result = f.query(right) - f.query(left);
        CAPTURE(i, left, right, result, expected[i]);
        REQUIRE(result == expected[i]);
    }
}

TEST_CASE("XorFenwickTree", "[fenwick_tree]") {
    // Calculate the xor of a all elements in a range.
    // Two types of queries:
    //    - 1: Range xor of interval [a, b) and 0-index.
    //    - 2: Update position p (0-index) with value v

    struct _xor {
        int val;

        _xor& operator+=(const _xor& other) {
            val ^= other.val;
            return *this;
        }
    };

    SECTION("3 queries: range -> update -> range") {
        std::vector<_xor> a = {_xor{2},
                               _xor{1},
                               _xor{1},
                               _xor{3},
                               _xor{2},
                               _xor{3},
                               _xor{4},
                               _xor{5},
                               _xor{6},
                               _xor{7},
                               _xor{8},
                               _xor{9}};
        std::vector<std::tuple<int, int, int>> queries = {{1, 3, 9}, {2, 5, 6}, {1, 3, 9}};
        std::vector<int> expected = {5, 3};

        fenwick<_xor> f = build_fenwick_tree(a);
        int Q = (int)queries.size(), cnt = 0;
        for(int i = 0; i < Q; ++i) {
            int t = std::get<0>(queries[i]);
            int left = std::get<1>(queries[i]);
            int right = std::get<2>(queries[i]);

            CAPTURE(i, cnt, t, left, right, result, expected[cnt]);
            if(t == 1) {
                int result = f.query(right).val ^ f.query(left).val;
                REQUIRE(result == expected[cnt]);
                ++cnt;
            }
            else {
                f.update(left, _xor{right});
            }
        }
    }

    SECTION("5 queries: range -> update -> update -> update -> range") {
        std::vector<_xor> a = {_xor{2},
                               _xor{1},
                               _xor{1},
                               _xor{3},
                               _xor{2},
                               _xor{3},
                               _xor{4},
                               _xor{5},
                               _xor{6},
                               _xor{7},
                               _xor{8},
                               _xor{9}};
        std::vector<std::tuple<int, int, int>> queries = {{1, 0, 10},
                                                          {2, 4, 6},
                                                          {2, 6, 5},
                                                          {2, 9, 1},
                                                          {1, 0, 10}};
        std::vector<int> expected = {0, 2};

        fenwick<_xor> f = build_fenwick_tree(a);
        int Q = (int)queries.size(), cnt = 0;
        for(int i = 0; i < Q; ++i) {
            int t = std::get<0>(queries[i]), left = std::get<1>(queries[i]),
                right = std::get<2>(queries[i]);

            CAPTURE(i, cnt, t, left, right, result, expected[cnt]);
            if(t == 1) {
                int result = f.query(right).val ^ f.query(left).val;
                REQUIRE(result == expected[cnt]);
                ++cnt;
            }
            else {
                f.update(left, _xor{right});
            }
        }
    }
}