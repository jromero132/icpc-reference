#include "code/data_structures/fenwick_tree.hpp"

#include <cassert>
#include <tuple>
#include <vector>

#include "testing.hpp"

using namespace std;

template <typename T>
fenwick<T> build_fenwick_tree(const std::vector<T> v) {
    const int N = (int)v.size();
    fenwick<T> f(N);
    for (int i = 0; i < N; ++i) {
        f.update(i + 1, v[i]);
    }
    return f;
}

void test_sum_queries() {
    testing::CURRENT_FUNC_NAME = __func__;

    // Calculate the sum of a all elements in a range.
    // Range is [a, b) and 0-index.

    const std::vector<int> a = {1, 10, 100, 1000, 10000};
    const std::vector<std::pair<int, int>> queries = {{2, 3}, {0, 3}, {2, 5}, {3, 4}, {0, 5}};
    const std::vector<int> expected = {100, 111, 11100, 1000, 11111};

    fenwick<int> f = build_fenwick_tree(a);
    const int Q = (int)queries.size();
    for (int i = 0; i < Q; ++i) {
        const int left = queries[i].first;
        const int right = queries[i].second;
        const int result = f.query(right) - f.query(left);
        assert(result == expected[i]);
    }
}

struct _xor {
    int val;

    _xor& operator+=(const _xor& other) {
        val ^= other.val;
        return *this;
    }
};

void test_xor_mixed_queries_and_updates_1() {
    testing::CURRENT_FUNC_NAME = __func__;

    // Calculate the xor of a all elements in a range.
    // Two types of queries:
    //    - 1: Range xor of interval [a, b) and 0-index.
    //    - 2: Update position p (0-index) with value v

    // 3 queries: range -> update -> range

    const std::vector<_xor> a = {_xor{2}, _xor{1}, _xor{1}, _xor{3}, _xor{2}, _xor{3},
                                 _xor{4}, _xor{5}, _xor{6}, _xor{7}, _xor{8}, _xor{9}};
    const std::vector<std::tuple<int, int, int>> queries = {{1, 3, 9}, {2, 5, 6}, {1, 3, 9}};
    const std::vector<int> expected = {5, 3};

    fenwick<_xor> f = build_fenwick_tree(a);
    const int Q = (int)queries.size();
    for (int i = 0, cnt = 0; i < Q; ++i) {
        const int t = std::get<0>(queries[i]);
        const int left = std::get<1>(queries[i]);
        const int right = std::get<2>(queries[i]);

        if (t == 1) {
            const int result = f.query(right).val ^ f.query(left).val;
            assert(result == expected[cnt]);
            ++cnt;
        }
        else {
            f.update(left, _xor{right});
        }
    }
}

void test_xor_mixed_queries_and_updates_2() {
    testing::CURRENT_FUNC_NAME = __func__;

    // Calculate the xor of a all elements in a range.
    // Two types of queries:
    //    - 1: Range xor of interval [a, b) and 0-index.
    //    - 2: Update position p (0-index) with value v

    // 5 queries: range -> update -> update -> update -> range

    const std::vector<_xor> a = {_xor{2}, _xor{1}, _xor{1}, _xor{3}, _xor{2}, _xor{3},
                                 _xor{4}, _xor{5}, _xor{6}, _xor{7}, _xor{8}, _xor{9}};
    const std::vector<std::tuple<int, int, int>> queries = {{1, 0, 10}, {2, 4, 6}, {2, 6, 5}, {2, 9, 1}, {1, 0, 10}};
    const std::vector<int> expected = {0, 2};

    fenwick<_xor> f = build_fenwick_tree(a);
    const int Q = (int)queries.size();
    for (int i = 0, cnt = 0; i < Q; ++i) {
        const int t = std::get<0>(queries[i]);
        const int left = std::get<1>(queries[i]);
        const int right = std::get<2>(queries[i]);

        if (t == 1) {
            const int result = f.query(right).val ^ f.query(left).val;
            assert(result == expected[cnt]);
            ++cnt;
        }
        else {
            f.update(left, _xor{right});
        }
    }
}

int main() {
    return testing::run_tests(
        {test_sum_queries, test_xor_mixed_queries_and_updates_1, test_xor_mixed_queries_and_updates_2}
    );
}