// =============================================================================
// @author     Jose A. Romero (jromero132)
// @unit_test  Sparse Table (RMQ)
// @docs       Comprehensive validation suite testing 0-based spatial boundaries,
//             duplicate uniform runs, negative numbers, and custom comparators
//             reconfigured to handle Range Maximum Queries (RMQ).
// =============================================================================

#include "../../code/data_structures/rmq.cpp"

#include <algorithm>
#include <functional>
#include <vector>

#include "../doctest.h"

// Explicit value combiner for Maximum queries to avoid bool comparison casting
template <typename T>
struct max_op {
  T operator()(const T& a, const T& b) const { return std::max(a, b); }
};

TEST_CASE("Sparse Table RMQ - Comprehensive Verification Suite") {
  SUBCASE("Single Element & Minimal Array Boundaries") {
    std::vector<int> single = {42};
    rmq<int> table(single.begin(), single.end());

    CHECK(table.query(0, 0) == 42);
  }

  SUBCASE("Standard Array Range Minimum Lookups") {
    std::vector<int> data = {10, 5, 20, 3, 15, 30, 1};
    rmq<int> table(data.begin(), data.end());

    // Evaluate subcase ranges inside data profile
    CHECK(table.query(0, 2) == 5);   // Min of {10, 5, 20}
    CHECK(table.query(2, 4) == 3);   // Min of {20, 3, 15}
    CHECK(table.query(0, 6) == 1);   // Min of entire array
    CHECK(table.query(4, 5) == 15);  // Min of {15, 30}
    CHECK(table.query(5, 5) == 30);  // Identity index tracking
  }

  SUBCASE("Uniform Duplicates and Monotonic Trends") {
    std::vector<int> uniform(100, 7);
    rmq<int> table_uniform(uniform.begin(), uniform.end());
    CHECK(table_uniform.query(10, 85) == 7);

    std::vector<int> descending = {10, 9, 8, 7, 6, 5, 4};
    rmq<int> table_desc(descending.begin(), descending.end());

    // FIXED: The minimum of elements {9, 8, 7, 6} in range [1, 4] is 6
    CHECK(table_desc.query(1, 4) == 6);
  }

  SUBCASE("Negative and Large Interleaved Values") {
    std::vector<int> negatives = {-5, -10, 0, -2, 15, -100, 25};
    rmq<int> table(negatives.begin(), negatives.end());

    CHECK(table.query(0, 3) == -10);
    CHECK(table.query(2, 6) == -100);
    CHECK(table.query(6, 6) == 25);
  }

  SUBCASE("Custom Functional Operator Transformation (Range Maximum Query)") {
    std::vector<int> data = {4, 12, 8, 23, 19, 5, 31, 14};

    // FIXED: Swapped std::greater<int> for a proper value-returning max_op functor
    rmq<int, max_op<int>> max_table(data.begin(), data.end());

    CHECK(max_table.query(0, 2) == 12);  // Max of {4, 12, 8}
    CHECK(max_table.query(2, 4) == 23);  // Max of {8, 23, 19}
    CHECK(max_table.query(0, 7) == 31);  // Global Maximum
    CHECK(max_table.query(4, 5) == 19);  // Max of {19, 5}
  }
}
