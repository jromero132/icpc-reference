#include <cassert>
#include <string>

#include "testing.hpp"

using namespace std;

void test_sanity_check_string() {  // Obvious string comparison
    testing::CURRENT_FUNC_NAME = __func__;
    string expected = "sanity check";
    assert("sanity check" == expected);
}

void test_sanity_check_calc() {  // Obvious integer comparison
    testing::CURRENT_FUNC_NAME = __func__;
    int expected = 132;
    assert(11 * 12 == expected);
}

int main() { return testing::run_tests({test_sanity_check_string, test_sanity_check_calc}); }