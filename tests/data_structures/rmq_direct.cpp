// =============================================================================
// @author     Jose A. Romero (jromero132)
// @unit_test  Direct Range Minimum Query (Fischer-Heun) Test Suite
// @docs       Validates constant-time O(1) minimum lookups across micro-ranges,
//             intra-block boundaries, inter-block crossings, and custom
//             inverted functor logic (Range Maximum Queries).
// =============================================================================

#include "../../code/data_structures/rmq_direct.cpp"

#include <string>
#include <vector>

#include "../doctest.h"

// Custom structure to verify maximum/minimum queries using custom comparisons
struct Request {
  int weight;
  std::string tag;

  bool operator<(const Request& o) const { return weight < o.weight; }
  bool operator>(const Request& o) const { return weight > o.weight; }
};

TEST_SUITE("Direct RMQ Suite") {
  TEST_CASE("Minimal Array Boundaries") {
    std::vector<int> a = {42};
    rmq_direct<int> rmq(a.begin(), a.end());

    SUBCASE("Single element query bounds") {
      CHECK(rmq(0, 1, a) == 0);
    }
  }

  TEST_CASE("Standard Monotonic Arrays") {
    std::vector<int> ascending = {10, 20, 30, 40, 50, 60, 70, 80};
    std::vector<int> descending = {80, 70, 60, 50, 40, 30, 20, 10};

    SUBCASE("Strictly ascending minimum mappings") {
      rmq_direct<int> rmq(ascending.begin(), ascending.end());
      CHECK(rmq(0, 4, ascending) == 0);
      CHECK(rmq(2, 6, ascending) == 2);
      CHECK(rmq(5, 8, ascending) == 5);
    }

    SUBCASE("Strictly descending minimum mappings") {
      rmq_direct<int> rmq(descending.begin(), descending.end());
      CHECK(rmq(0, 4, descending) == 3);
      CHECK(rmq(2, 7, descending) == 6);
      CHECK(rmq(0, 8, descending) == 7);
    }
  }

  TEST_CASE("Duplicate Elements & Platform Stability") {
    // Elements at index 1 and index 4 share identical minimum values
    std::vector<int> duplicates = {5, 2, 7, 8, 2, 9};
    rmq_direct<int> rmq(duplicates.begin(), duplicates.end());

    SUBCASE("Tie-breaking range checks") {
      // Should return the first occurrence or consistent internal mapping
      int res = rmq(0, 6, duplicates);
      CHECK(duplicates[res] == 2);

      // Sub-range queries covering isolated duplicates
      CHECK(rmq(3, 6, duplicates) == 4);
      CHECK(rmq(0, 3, duplicates) == 1);
    }
  }

  TEST_CASE("Block Crossings & Micro Queries") {
    // Long array to enforce block division structures
    std::vector<int> a = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 11, 12, 13, -5, 14, 15};
    rmq_direct<int> rmq(a.begin(), a.end());

    SUBCASE("Intra-block query boundaries") {
      CHECK(a[rmq(0, 3, a)] == 7);
    }

    SUBCASE("Inter-block wide crossing span queries") {
      CHECK(rmq(0, 16, a) == 13);  // Global minimum -5 at index 13
      CHECK(rmq(0, 10, a) == 9);   // Value 0 at index 9
    }
  }

  TEST_CASE("Custom Type & Inverted Logic Criteria") {
    std::vector<Request> items = {{50, "A"}, {10, "B"}, {90, "C"}, {5, "D"}};

    SUBCASE("Range Minimum Lookup using standard less comparison") {
      rmq_direct<Request> rmq(items.begin(), items.end());
      CHECK(rmq(0, 3, items) == 1);  // Returns index of weight 10
      CHECK(rmq(0, 4, items) == 3);  // Returns index of weight 5
    }

    SUBCASE("Range Maximum Lookup using greater functor") {
      rmq_direct<Request, std::greater<Request>> rmq_max(items.begin(), items.end());
      CHECK(rmq_max(0, 4, items) == 2);  // Returns index of weight 90
    }
  }
}
