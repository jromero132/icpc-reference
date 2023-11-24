#include <vector>

#include "code/data_structures/rmq_direct.hpp"
#include "testing.hpp"

using namespace std;

void test_rmq_direct_simple_queries() {
    testing::CURRENT_FUNC_NAME = __func__;

    const vector<int> data = {2, 10, 1, 100};
    const rmq_direct<int> rmq(data.begin(), data.end());

    const vector<vector<int>> queries = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2},
                                         {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}};
    const vector<int> expected_results = {0, 0, 2, 2, 1, 2, 2, 2, 2, 3};

    for (int i = 0; i < queries.size(); ++i) {
        const int l = queries[i][0], r = queries[i][1];
        const int expected_result = expected_results[i];
        const int result = rmq(l, r, data);

        CAPTURE_VARS(l, r, result, expected_result);
        assert(result == expected_result);
    }
}

void test_rmq_direct_random_queries() {
    testing::CURRENT_FUNC_NAME = __func__;

    const vector<int> data = {2, 10, 1, 100};
    const rmq_direct<int> rmq(data.begin(), data.end());

    const vector<vector<int>> queries = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2},
                                         {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}};
    const vector<int> expected_results = {0, 0, 2, 2, 1, 2, 2, 2, 2, 3};

    for (int i = 0; i < queries.size(); ++i) {
        const int l = queries[i][0], r = queries[i][1];
        const int expected_result = expected_results[i];
        const int result = rmq(l, r, data);

        CAPTURE_VARS(l, r, result, expected_result);
        assert(result == expected_result);
    }
}

int main() { return testing::run_tests({test_rmq_direct_simple_queries}); }