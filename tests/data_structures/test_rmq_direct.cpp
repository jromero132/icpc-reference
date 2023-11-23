#include <cassert>
#include <vector>

#include "code/data_structures/rmq_direct.hpp"
#include "testing.hpp"

using namespace std;

void test_rmq_direct_simple_queries() {
    testing::CURRENT_FUNC_NAME = __func__;

    vector<int> data = {2, 10, 1, 100};
    rmq_direct<int> rmq(data.begin(), data.end());

    vector<vector<int>> queries = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}};
    vector<int> expected = {0, 0, 2, 2, 1, 2, 2, 2, 2, 3};

    for (int i = 0; i < queries.size(); ++i) {
        int l = queries[i][0], r = queries[i][1];
        assert(rmq(l, r, data) == expected[i]);
    }
}

int main() { return testing::run_tests({test_rmq_direct_simple_queries}); }