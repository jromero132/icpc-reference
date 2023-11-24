#include <stdbool.h>

#include <algorithm>
#include <vector>

#include "code/data_structures/avl.hpp"
#include "code/utilities/random.hpp"
#include "testing.hpp"

using namespace std;

void test_avl_queries() {
    testing::CURRENT_FUNC_NAME = __func__;

    avl<int> tree({1, 5, 3});
    vector<int> in_order = tree.in_order();

    // Sub-test 1
    int value1 = 3;
    const int expected_order1 = 1;
    const int expected_value1 = 3;
    const int order1 = tree.order_of_key(value1);

    CAPTURE_VARS(in_order, value1, order1, expected_order1);
    assert(order1 == expected_order1);

    value1 = tree.kth(order1);
    CAPTURE_VARS(in_order, order1, value1, expected_value1);
    assert(value1 == expected_value1);

    // Sub-test 2
    int value2 = 0;
    const int expected_order2 = 0;
    const int expected_value2 = 1;
    const int order2 = tree.order_of_key(value2);

    CAPTURE_VARS(in_order, value2, order2, expected_order2);
    assert(order2 == expected_order2);

    value2 = tree.kth(order2);
    CAPTURE_VARS(in_order, order2, value2, expected_value2);
    assert(value2 == expected_value2);

    // Insertions and deletions
    tree.insert(3);
    tree.insert(4);
    tree.insert(3);
    tree.insert(4);
    tree.erase(3);
    in_order = tree.in_order();

    // Sub-test 3
    const int value3 = 3;
    const bool expected_result3 = false;
    const bool result3 = tree.find(value3);

    CAPTURE_VARS(in_order, value3, result3, expected_result3);
    assert(tree.find(value3) == expected_result3);

    // Sub-test 4
    const int value4 = 4;
    const bool expected_result4 = true;
    const bool result4 = tree.find(value4);

    CAPTURE_VARS(in_order, value3, result3, expected_result3);
    assert(tree.find(value4) == expected_result4);

    // Sub-test 5
    const vector<int> values = {0, 1, 2, 3, 4, 5, 6};
    const vector<int> expected_orders = {0, 0, 1, 1, 1, 2, 3};
    for (int i = 0; i < values.size(); ++i) {
        const int value = values[i];
        const int expected_order = expected_orders[i];
        const int order = tree.order_of_key(value);

        CAPTURE_VARS(in_order, value, order, expected_order);
        assert(order == expected_order);
    }

    // Sub-test 6
    const vector<int> expected_in_order = {1, 4, 5};
    in_order = tree.in_order();

    CAPTURE_VARS(in_order, expected_in_order);
    assert(in_order == expected_in_order);
}

template <typename T = int>
void base_avl_lots_of_rotations_test(const avl<T>& tree, const vector<T>& values) {
    const vector<T> in_order = tree.in_order();

    CAPTURE_VARS(values.size(), tree.root->sz, in_order, values);
    assert(tree.root->sz == values.size() && in_order.size() == values.size());

    for (int i = 0; i < in_order.size(); ++i) {
        if (!(in_order[i] == values[i])) {
            CAPTURE_VARS(i, in_order, values);
        }
        assert(in_order[i] == values[i]);
    }

    for (int i = 0; i < values.size(); ++i) {
        T value = values[i];

        const int expected_found = true;
        const int expected_order = i;
        const T expected_value = value;

        const int found = tree.find(value);
        if (!(found == expected_found)) {
            CAPTURE_VARS(in_order, values, value, found, expected_found);
        }
        assert(found == expected_found);

        const int order = tree.order_of_key(value);
        if (!(order == expected_order)) {
            CAPTURE_VARS(in_order, values, value, order, expected_order);
        }
        assert(order == expected_order);

        value = tree.kth(order);
        if (!(value == expected_value)) {
            CAPTURE_VARS(in_order, values, order, value, expected_value);
        }
        assert(value == expected_value);

        value = tree.kth(order + 1, 1);
        if (!(value == expected_value)) {
            CAPTURE_VARS(in_order, values, order + 1, value, expected_value);
        }
        assert(value == expected_value);
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

    const int amount_to_remove = randint(1, MAX_N - MIN_N);
    values.resize(values.size() - amount_to_remove);

    avl<int> tree;
    for (auto v : values) {
        tree.insert(v);
    }
    sort(values.begin(), values.end());
    base_avl_lots_of_rotations_test(tree, values);
}

struct my_int {
    int v;

    my_int(int v) : v(v) {}
    my_int() : my_int(0) {}

    bool operator==(const my_int& other) const { return v == other.v; }
    bool operator!=(const my_int& other) const { return !(*this == other); }
    bool operator<(const my_int& other) const { return other.v < v; }
    bool operator>(const my_int& other) const { return other < *this; }
    bool operator<=(const my_int& other) const { return *this < other || *this == other; }
    bool operator>=(const my_int& other) const { return !(*this < other); }
};

ostream& operator<<(std::ostream& os, const my_int& value) {
    os << value.v;
    return os;
}

void test_avl_lots_of_random_rotations_with_random_values_reverse_order() {
    testing::CURRENT_FUNC_NAME = __func__;

    const int MIN_N = 1, MAX_N = 1000000;
    vector<my_int> values;
    for (int i = MIN_N; i <= MAX_N; ++i) {
        values.push_back(my_int(i));
    }
    shuffle(values.begin(), values.end(), rng);

    const int amount_to_remove = randint(1, MAX_N - MIN_N);
    values.resize(values.size() - amount_to_remove);

    avl<my_int> tree;
    for (auto v : values) {
        tree.insert(v);
    }
    sort(values.begin(), values.end());
    base_avl_lots_of_rotations_test(tree, values);
}

int main() {
    return testing::run_tests(
        {test_avl_queries, test_avl_lots_of_left_rotations, test_avl_lots_of_right_rotations,
         test_avl_lots_of_random_rotations, test_avl_lots_of_random_rotations_with_random_values_reverse_order}
    );
}