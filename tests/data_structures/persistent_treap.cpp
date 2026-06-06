// =============================================================================
// @author     Jose A. Romero (jromero132)
// @unit_test  Persistent Treap Verification Suite
// @docs       Validates strict persistence isolation boundaries, rank lookups,
//             k-th selection indices, empty states, and functional parity
//             between standard rotation and split-merge pipelines.
// =============================================================================

#include "../../code/data_structures/persistent_treap.cpp"

#include <algorithm>
#include <vector>

#include "../doctest.h"

TEST_CASE("Persistent Treap - Comprehensive Verification Suite") {
  treap<int> t;

  SUBCASE("Dummy & Edge Cases: Empty Tree Invariants") {
    treap<int>::node* root = nullptr;
    CHECK(t.size(root) == 0);
    CHECK(t.less(root, 42) == 0);
    CHECK(t.kth(root, 1) == nullptr);
  }

  SUBCASE("Good Cases: Functional Immutability & Persistence") {
    treap<int>::node* v0 = nullptr;
    treap<int>::node* v1 = t.insert(v0, 40);
    treap<int>::node* v2 = t.insert(v1, 20);
    treap<int>::node* v3 = t.insert(v2, 50);
    treap<int>::node* v4 = t.insert(v3, 30);

    // Verify isolation and sizes across all structural history versions
    REQUIRE(t.size(v0) == 0);
    REQUIRE(t.size(v1) == 1);
    REQUIRE(t.size(v2) == 2);
    REQUIRE(t.size(v3) == 3);
    REQUIRE(t.size(v4) == 4);

    // Verify Version 2 properties (contains sorted: 20, 40)
    REQUIRE(t.kth(v2, 1) != nullptr);
    CHECK(t.kth(v2, 1)->key == 20);
    REQUIRE(t.kth(v2, 2) != nullptr);
    CHECK(t.kth(v2, 2)->key == 40);

    // Verify Version 4 properties (contains sorted: 20, 30, 40, 50)
    CHECK(t.kth(v4, 1)->key == 20);
    CHECK(t.kth(v4, 2)->key == 30);
    CHECK(t.kth(v4, 3)->key == 40);
    CHECK(t.kth(v4, 4)->key == 50);

    // Verify order statistics rank queries (less) on Version 4
    CHECK(t.less(v4, 15) == 0);
    CHECK(t.less(v4, 25) == 1);  // 20 is smaller
    CHECK(t.less(v4, 35) == 2);  // 20, 30 are smaller
    CHECK(t.less(v4, 60) == 4);  // All elements are smaller
  }

  SUBCASE("Tricky Cases: Dual Implementation Alignment & Erasures") {
    treap<int>::node* root1 = nullptr;
    treap<int>::node* root2 = nullptr;

    std::vector<int> elements = {15, 8, 23, 4, 11};
    for (size_t i = 0; i < elements.size(); ++i) {
      root1 = t.insert(root1, elements[i]);   // Built via rotation interface
      root2 = t.insert2(root2, elements[i]);  // Built via split-merge interface
    }

    // Verify structural configuration and value sorting parity
    REQUIRE(t.size(root1) == 5);
    REQUIRE(t.size(root2) == 5);
    for (int i = 1; i <= 5; ++i) {
      CHECK(t.kth(root1, i)->key == t.kth(root2, i)->key);
    }

    // Perform target erasure on both structures independently
    treap<int>::node* del1 = t.erase(root1, 8);
    treap<int>::node* del2 = t.erase2(root2, 8);

    CHECK(t.size(del1) == 4);
    CHECK(t.size(del2) == 4);

    // Sorted result should remain consistent: 4, 11, 15, 23
    CHECK(t.kth(del1, 2)->key == 11);
    CHECK(t.kth(del2, 2)->key == 11);

    // Verify historical versions remain immutable
    CHECK(t.size(root1) == 5);
    CHECK(t.size(root2) == 5);
    CHECK(t.kth(root1, 2)->key == 8);
  }
}
