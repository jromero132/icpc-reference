#include "../../code/data_structures/fenwick.cpp"

#include <climits>

#include "../doctest.h"

TEST_SUITE("Fenwick Tree Suite") {
  TEST_CASE("Initialization and Empty Tree State") {
    fenwick<int> ft(5);

    SUBCASE("All elements should initially query to zero") {
      for (int i = 1; i <= 5; ++i) {
        CHECK(ft.query(i) == 0);
      }
    }

    SUBCASE("Range queries on empty tree must be zero") {
      CHECK(ft.query(1, 5) == 0);
      CHECK(ft.query(3, 3) == 0);
    }
  }

  TEST_CASE("Point Updates and Prefix Sum Tracking") {
    fenwick<long long> ft(4);

    // Simulating sequence layout
    ft.update(1, 3);   // [3, 0, 0, 0]
    ft.update(2, 5);   // [3, 5, 0, 0]
    ft.update(3, -2);  // [3, 5, -2, 0]
    ft.update(4, 10);  // [3, 5, -2, 10]

    SUBCASE("Prefix evaluation queries match exactly") {
      CHECK(ft.query(1) == 3);
      CHECK(ft.query(2) == 8);
      CHECK(ft.query(3) == 6);
      CHECK(ft.query(4) == 16);
    }

    SUBCASE("Subsegment range operations function correctly") {
      CHECK(ft.query(2, 3) == 3);   // 5 + (-2)
      CHECK(ft.query(1, 4) == 16);  // Total accumulation
      CHECK(ft.query(3, 3) == -2);  // Single cell range mapping
    }
  }

  TEST_CASE("Boundary Conditions and Extreme Constraints") {
    // Checking extreme size constraint boundaries
    fenwick<long long> ft(1);

    SUBCASE("Single element tree operations execution") {
      ft.update(1, 42);
      CHECK(ft.query(1) == 42);
      CHECK(ft.query(1, 1) == 42);
    }

    SUBCASE("Integer Overflow Protection") {
      fenwick<long long> ft_large(2);
      ft_large.update(1, 2000000000LL);
      ft_large.update(2, 2000000000LL);

      // Should overflow regular 32-bit integers but hold fine in long long
      CHECK(ft_large.query(2) == 4000000000LL);
    }
  }

  TEST_CASE("Destructive Balance Checks") {
    fenwick<int> ft(3);
    ft.update(2, 100);

    SUBCASE("Inverse updates can cancel out to zero perfectly") {
      ft.update(2, -100);
      CHECK(ft.query(2) == 0);
      CHECK(ft.query(2, 2) == 0);
    }
  }
}
