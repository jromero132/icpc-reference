#include <string>
#include <vector>

#include "testing.hpp"

using namespace std;

void test_sanity_check_string() {  // Obvious string comparison
    testing::CURRENT_FUNC_NAME = __func__;

    string expected = "sanity check";
    assert("sanity check" == expected);
}

void test_sanity_check_calc() {  // Obvious integer comparison
    testing::CURRENT_FUNC_NAME = __func__;

    int a = 11;
    int b = 12;
    int expected = 132;

    CAPTURE_VARS(a, b, expected);
    assert(a * b == expected);
}

void test_sanity_check_vector() {
    testing::CURRENT_FUNC_NAME = __func__;

    vector<int> v1 = {1, 3, 2};
    vector<int> v2 = v1;
    CAPTURE_VARS(v1, v2);
    assert(v1 == v2);

    v1[2] = 132;
    CAPTURE_VARS(v1, v2);
    assert(v1 != v2);

    vector<vector<int>> v3 = {{1, 3, 2}, {13, 2, 132}};
    vector<vector<int>> v4 = v3;
    CAPTURE_VARS(v3, v4);
    assert(v3 == v4);

    v3[0][2] = 132;
    CAPTURE_VARS(v3, v4);
    assert(v3 != v4);
}

int main() { return testing::run_tests({test_sanity_check_string, test_sanity_check_calc, test_sanity_check_vector}); }