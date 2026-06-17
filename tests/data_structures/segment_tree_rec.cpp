// =============================================================================
// @author     Jose A. Romero (jromero132)
// @unit_test  Segment Tree with Lazy Propagation (Generic Testing)
// @docs       Validates correctness across standard sum queries, empty ranges,
//             negative integers, and custom alternative structural policies.
// =============================================================================

#include <algorithm>
#include <vector>

#include "../doctest.h"

namespace st_rec {
#include "../../code/data_structures/segment_tree_rec.cpp"

// Functors designed to configure a Range Maximum Query with Range Addition Tree
struct max_combine {
  int operator()(int a, int b) const { return std::max(a, b); }
};
struct max_lazy_apply {
  void operator()(int& tree_val, int lazy_val, int len) const {
    (void)len;  // For maximum queries, interval length does not scale addition
    tree_val += lazy_val;
  }
};
}  // namespace st_rec

TEST_CASE("Lazy Segment Tree - Verification Suite") {
  SUBCASE("Dummy & Trivial Boundary Edge Cases") {
    st_rec::segment_tree<long long> st(1, 0, 0);
    CHECK(st.query(1, 1) == 0);

    st.update(1, 1, 10);
    CHECK(st.query(1, 1) == 10);

    // Invalid/Empty interval query checks
    CHECK(st.query(3, 1) == 0);
  }

  SUBCASE("Standard RSQ Accumulations & Overlaps") {
    st_rec::segment_tree<long long> st(10, 0, 0);

    st.update(1, 10, 3);  // State: all values equal 3
    CHECK(st.query(1, 10) == 30);
    CHECK(st.query(1, 4) == 12);

    st.update(4, 7, 2);  // Add +2 to indices 4, 5, 6, 7
    // State values: [3, 3, 3, 5, 5, 5, 5, 3, 3, 3]
    CHECK(st.query(1, 10) == 38);
    CHECK(st.query(3, 5) == 13);  // 3 + 5 + 5
    CHECK(st.query(7, 9) == 11);  // 5 + 3 + 3
  }

  SUBCASE("Negative Modifiers & Absolute Integrities") {
    st_rec::segment_tree<int> st(5, 0, 0);
    st.update(1, 5, 20);
    st.update(2, 4, -30);

    // State values: [20, -10, -10, -10, 20]
    CHECK(st.query(1, 5) == 10);
    CHECK(st.query(2, 4) == -30);
    CHECK(st.query(4, 5) == 10);
  }

  SUBCASE("Custom Alternative Policy Validation (Generic Range Max Query)") {
    // Identity elements: tree identity = -1e9 (infinity min), lazy identity = 0
    st_rec::segment_tree<int, st_rec::max_combine, st_rec::max_lazy_apply, st_rec::default_lazy_merge<int>>
        st_max(5, -1000000000, 0);

    // Manually baseline array from 0 to actual starting points:
    st_max.update(1, 1, 1000000005);  // Ind 1 = 5
    st_max.update(2, 2, 1000000012);  // Ind 2 = 12
    st_max.update(3, 3, 1000000002);  // Ind 3 = 2
    st_max.update(4, 4, 1000000025);  // Ind 4 = 25
    st_max.update(5, 5, 1000000009);  // Ind 5 = 9
    // Current Array State: [5, 12, 2, 25, 9]

    CHECK(st_max.query(1, 5) == 25);
    CHECK(st_max.query(1, 3) == 12);

    st_max.update(2, 4, 10);
    // New Array State: [5, 22, 12, 35, 9]
    CHECK(st_max.query(1, 5) == 35);
    CHECK(st_max.query(1, 3) == 22);
    CHECK(st_max.query(4, 5) == 35);
  }
}
