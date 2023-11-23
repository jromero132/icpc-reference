#include <algorithm>
#include <cassert>
#include <vector>

#include "code/data_structures/avl.hpp"
#include "code/utilities/random.hpp"
#include "testing.hpp"

using namespace std;

void test_avl_queries() {
    testing::CURRENT_FUNC_NAME = __func__;

    avl<int> tree({1, 5, 3});

    int value1 = 3;
    int expected_order1 = 1;
    int expected_value1 = 3;
    assert(tree.order_of_key(value1) == expected_order1);
    assert(tree.kth(expected_order1) == expected_value1);

    int value2 = 0;
    int expected_order2 = 0;
    int expected_value2 = 1;
    assert(tree.order_of_key(value2) == expected_order2);
    assert(tree.kth(expected_order2) == expected_value2);

    tree.insert(3);
    tree.insert(4);
    tree.insert(3);
    tree.insert(4);
    tree.erase(3);

    assert(!tree.find(3));
    assert(tree.find(4));

    vector<int> values = {0, 1, 2, 3, 4, 5, 6};
    vector<int> expected_order = {0, 0, 1, 1, 1, 2, 3};
    for (int i = 0; i < values.size(); ++i) {
        assert(tree.order_of_key(values[i]) == expected_order[i]);
    }

    vector<int> expected_in_order = {1, 4, 5};
    assert(tree.in_order() == expected_in_order);
}

void base_avl_lots_of_rotations_test(const avl<int>& tree, const vector<int>& values) {
    assert(tree.root->sz == values.size());

    for (int i = 0; i < values.size(); ++i) {
        assert(tree.find(values[i]));
        assert(tree.order_of_key(values[i]) == i);
        assert(tree.kth(i) == values[i]);
        assert(tree.kth(i + 1, 1) == values[i]);
    }

    vector<int> in_order = tree.in_order();
    assert(in_order.size() == values.size());
    for (int i = 0; i < in_order.size(); ++i) {
        assert(in_order[i] == values[i]);
    }
}

void base_avl_lots_of_rotations_test(const avl<int>& tree, int start, int end) {
    if (start > end) swap(start, end);

    vector<int> values;
    for (int i = start; i <= end; ++i) {
        values.push_back(i);
    }
    base_avl_lots_of_rotations_test(tree, values);
}

void test_avl_lots_of_left_rotations() {
    testing::CURRENT_FUNC_NAME = __func__;

    const int MIN_N = 1, MAX_N = 1000000;
    avl<int> tree;
    for (int i = MIN_N; i <= MAX_N; ++i) {
        tree.insert(i);
    }
    base_avl_lots_of_rotations_test(tree, MIN_N, MAX_N);
}

void test_avl_lots_of_right_rotations() {
    testing::CURRENT_FUNC_NAME = __func__;

    const int MIN_N = 1, MAX_N = 1000000;
    avl<int> tree;
    for (int i = MAX_N; i >= MIN_N; --i) {
        tree.insert(i);
    }
    base_avl_lots_of_rotations_test(tree, MIN_N, MAX_N);
}

void test_avl_lots_of_random_rotations() {
    testing::CURRENT_FUNC_NAME = __func__;

    const int MIN_N = 1, MAX_N = 1000000;
    vector<int> values;
    for (int i = MIN_N; i <= MAX_N; ++i) {
        values.push_back(i);
    }
    shuffle(values.begin(), values.end(), rng);

    avl<int> tree;
    for (auto v : values) {
        tree.insert(v);
    }
    base_avl_lots_of_rotations_test(tree, MIN_N, MAX_N);
}

void test_avl_lots_of_random_rotations_with_random_values() {
    testing::CURRENT_FUNC_NAME = __func__;

    const int MIN_N = 1, MAX_N = 1000000;
    vector<int> values;
    for (int i = MIN_N; i <= MAX_N; ++i) {
        values.push_back(i);
    }
    shuffle(values.begin(), values.end(), rng);
    int amount_to_remove = randint(1, MAX_N - MIN_N);
    values.resize(values.size() - amount_to_remove);

    avl<int> tree;
    for (auto v : values) {
        tree.insert(v);
    }
    sort(values.begin(), values.end());
    base_avl_lots_of_rotations_test(tree, values);
}

int main() {
    return testing::run_tests(
        {test_avl_queries, test_avl_lots_of_left_rotations, test_avl_lots_of_right_rotations,
         test_avl_lots_of_random_rotations}
    );
}