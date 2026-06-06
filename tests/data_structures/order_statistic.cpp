// =============================================================================
// @author     Jose A. Romero (jromero132)
// @unit_test  Policy-Based Ordered Set & Priority Queue Alias Layout
// @docs       Verifies the correctness of rank lookups (order_of_key),
//             positional indexing (find_by_order), duplicate rejection
//             behaviors, and multi-heap merging (join) using the doctest
//             framework.
// =============================================================================

#include "../../code/data_structures/order_statistic.cpp"

#include <algorithm>
#include <random>
#include <vector>

#include "../doctest.h"

TEST_CASE("Order Statistic & Heap Alias - Comprehensive Verification Suite") {
  SUBCASE("Order Statistic Set: Empty Boundaries & Invariants") {
    order_statistic_set<int> oss;
    CHECK(oss.empty() == true);
    CHECK(oss.size() == 0);
    CHECK(oss.order_of_key(42) == 0);
    CHECK(oss.find_by_order(0) == oss.end());
  }

  SUBCASE("Order Statistic Set: Functional Interleaved Queries") {
    order_statistic_set<int> oss;
    std::vector<int> vals = {40, 10, 30, 20, 50};
    for (int x : vals) oss.insert(x);

    // Expected sorted order sequence: 10, 20, 30, 40, 50
    REQUIRE(oss.size() == 5);

    CHECK(*oss.find_by_order(0) == 10);
    CHECK(*oss.find_by_order(2) == 30);
    CHECK(*oss.find_by_order(4) == 50);
    CHECK(oss.find_by_order(5) == oss.end());

    CHECK(oss.order_of_key(5) == 0);
    CHECK(oss.order_of_key(10) == 0);
    CHECK(oss.order_of_key(25) == 2);  // 10 and 20 are strictly smaller
    CHECK(oss.order_of_key(60) == 5);
  }

  SUBCASE("Order Statistic Set: Unique Rejection and Deletions") {
    order_statistic_set<int> oss;
    oss.insert(100);
    oss.insert(100);
    CHECK(oss.size() == 1);  // Set variants enforce uniqueness

    oss.insert(200);
    oss.insert(300);
    oss.erase(200);

    REQUIRE(oss.size() == 2);
    CHECK(*oss.find_by_order(1) == 300);
    CHECK(oss.order_of_key(300) == 1);
  }

  SUBCASE("Heap Structures: Priority Sorting & Merging Combinatorics") {
    heap<int> main_heap;
    heap<int> aux_heap;

    main_heap.push(15);
    main_heap.push(5);
    aux_heap.push(25);
    aux_heap.push(10);

    REQUIRE(main_heap.size() == 2);
    REQUIRE(aux_heap.size() == 2);

    // Joining pools together
    main_heap.join(aux_heap);

    CHECK(main_heap.size() == 4);
    CHECK(aux_heap.empty() == true);

    // Verifying priority max-heap ordering constraints
    std::vector<int> extracted;
    while (!main_heap.empty()) {
      extracted.push_back(main_heap.top());
      main_heap.pop();
    }

    std::vector<int> expected = {25, 15, 10, 5};
    CHECK(extracted == expected);
  }
}
