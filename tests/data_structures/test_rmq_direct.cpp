#include <vector>

#include "code/data_structures/rmq_direct.hpp"
#include "code/utilities/random.hpp"
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

    const int MIN_SIZE = 1000, MAX_SIZE = 10000;
    const int MIN_N = -1000000000, MAX_N = 1000000000;
    const int Q = 1000;

    const int n = randint(MIN_SIZE, MAX_SIZE);
    vector<int> data;
    for (int i = 0; i < n; ++i) {
        data.push_back(randint(MIN_N, MAX_N));
    }

    const rmq_direct<int> rmq(data.begin(), data.end());

    auto get_min = [&data](int l, int r) {
        int ans = data[l];
        for (++l; l < r; ++l) {
            ans = min(ans, data[l]);
        }
        return ans;
    };

    for (int i = 0; i < Q; ++i) {
        const int l = randint(0, n - 1), r = randint(l + 1, n);
        const int expected_result = get_min(l, r);
        const int result = data[rmq(l, r, data)];

        if (!(result == expected_result)) {
            CAPTURE_VARS(data, l, r, result, expected_result);
        }
        assert(result == expected_result);
    }
}

int main() { return testing::run_tests({test_rmq_direct_simple_queries, test_rmq_direct_random_queries}); }