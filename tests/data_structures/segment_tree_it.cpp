// =============================================================================
// @author     Jose A. Romero (jromero132)
// @unit_test  Iterative Segment Tree (0-Indexed [l, r) Verification)
// @docs       Validates correctness across trivial base sizes, empty ranges,
//             negative element ranges, point assignments, and custom algebraic functors.
// =============================================================================

#include <algorithm>
#include <vector>

#include "../doctest.h"

namespace st_it {
#include "../../code/data_structures/segment_tree_it.cpp"

// Functor designed to configure a Range Maximum Query Tree
struct max_combine {
  int operator()(int a, int b) const { return std::max(a, b); }
};
}  // namespace st_it

TEST_CASE("Iterative Segment Tree - 0-Indexed [l, r) Verification Suite") {
  SUBCASE("Dummy & Trivial Single-Element Boundaries") {
    st_it::segment_tree<int> st(1, 0);
    // [0, 1) covers only index 0
    CHECK(st.query(0, 1) == 0);

    st.update(0, 42, true);
    CHECK(st.query(0, 1) == 42);

    // Empty ranges or inverted boundaries should return identity
    CHECK(st.query(0, 0) == 0);
    CHECK(st.query(1, 0) == 0);
  }

  SUBCASE("Standard RSQ Accumulations & Point Modifier Operations") {
    st_it::segment_tree<long long> st(6, 0);

    st.update(0, 10, true);
    st.update(1, 20, true);
    st.update(2, 30, true);
    st.update(3, 40, true);
    st.update(4, 50, true);
    st.update(5, 60, true);
    // Internal values: [10, 20, 30, 40, 50, 60]

    CHECK(st.query(0, 6) == 210);
    CHECK(st.query(1, 4) == 90);  // index 1, 2, 3 -> 20 + 30 + 40

    // Combine-update modification test
    st.update(2, 5);  // Add 5 to index 2 -> value becomes 35
    CHECK(st.query(2, 3) == 35);
    CHECK(st.query(1, 4) == 95);

    // Pure assignment overwrite test
    st.update(3, 100, true);       // Index 3 becomes 100 instead of 40
    CHECK(st.query(2, 5) == 185);  // 35 + 100 + 50
  }

  SUBCASE("Negative Modifiers & Identity Bound Integrities") {
    st_it::segment_tree<int> st(4, 0);
    st.update(0, -5, true);
    st.update(1, -15, true);
    st.update(2, 10, true);
    st.update(3, -2, true);

    CHECK(st.query(0, 4) == -12);
    CHECK(st.query(0, 2) == -20);
    CHECK(st.query(1, 3) == -5);
  }

  SUBCASE("Custom Policy Validation (Generic Range Max Query)") {
    // Identity elements: tree identity = -2e9
    st_it::segment_tree<int, st_it::max_combine> st_max(5, -2000000000);

    st_max.update(0, 15, true);
    st_max.update(1, 45, true);
    st_max.update(2, 2, true);
    st_max.update(3, 89, true);
    st_max.update(4, 31, true);
    // Current Array State: [15, 45, 2, 89, 31]

    CHECK(st_max.query(0, 5) == 89);
    CHECK(st_max.query(0, 3) == 45);
    CHECK(st_max.query(2, 3) == 2);

    // Overwrite high value with a small one
    st_max.update(3, -10, true);
    // New Array State: [15, 45, 2, -10, 31]
    CHECK(st_max.query(0, 5) == 45);
    CHECK(st_max.query(2, 5) == 31);
  }
}
