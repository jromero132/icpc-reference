// =============================================================================
// @author     Jose A. Romero (jromero132)
// @unit_test  Chained Hash Table Test Suite
// @docs       Validates amortized O(1) map operations, entry re-writing, deep
//             bucket collisions, deletion chain link patching, and modulo-safe
//             negative signed integer key tracking.
// =============================================================================

#include "../../code/data_structures/hash_table.cpp"

#include <string>

#include "../doctest.h"

TEST_SUITE("Hash Table Suite") {
  TEST_CASE("Basic Key Operations") {
    hash_table<int, std::string> ht;

    SUBCASE("Lookup on non-existent targets") {
      CHECK(ht.find(100) == nullptr);
    }

    SUBCASE("Insertion and basic retrieval mapping") {
      ht.set(100, "ValueA");
      auto* res = ht.find(100);
      REQUIRE(res != nullptr);
      CHECK(*res == "ValueA");
    }

    SUBCASE("Updating an existing value key link") {
      ht.set(100, "ValueA");
      ht.set(100, "ValueB");  // Re-write assignment step
      auto* res = ht.find(100);
      REQUIRE(res != nullptr);
      CHECK(*res == "ValueB");
    }
  }

  TEST_CASE("Tricky Edge Cases & Modulo Safety Checks") {
    hash_table<int, std::string> ht;

    SUBCASE("Handling negative integer hashes") {
      ht.set(-505, "Negative Value");
      auto* res = ht.find(-505);
      REQUIRE(res != nullptr);
      CHECK(*res == "Negative Value");
    }

    SUBCASE("Handling zero index keys") {
      ht.set(0, "Zero Center");
      auto* res = ht.find(0);
      REQUIRE(res != nullptr);
      CHECK(*res == "Zero Center");
    }
  }

  TEST_CASE("Collision Resolution and Erase Mechanics") {
    hash_table<int, char> ht;

    // Compute intentional colliding items utilizing structure MOD bounds
    int first_key = 10;
    int colliding_key = 10 + hash_table<int, char>::MOD;

    ht.set(first_key, 'X');
    ht.set(colliding_key, 'Y');

    SUBCASE("Retrieving distinct items sharing identical buckets") {
      auto* p1 = ht.find(first_key);
      auto* p2 = ht.find(colliding_key);
      REQUIRE(p1 != nullptr);
      REQUIRE(p2 != nullptr);
      CHECK(*p1 == 'X');
      CHECK(*p2 == 'Y');
    }

    SUBCASE("Removing head element within a hash collision bucket chain") {
      // Because entries append to the head of the chain, colliding_key is head
      ht.erase(colliding_key);
      CHECK(ht.find(colliding_key) == nullptr);

      // Verify trailing chains remain intact
      auto* trailing = ht.find(first_key);
      REQUIRE(trailing != nullptr);
      CHECK(*trailing == 'X');
    }

    SUBCASE("Removing tail element within a hash collision bucket chain") {
      ht.erase(first_key);
      CHECK(ht.find(first_key) == nullptr);

      auto* head_node = ht.find(colliding_key);
      REQUIRE(head_node != nullptr);
      CHECK(*head_node == 'Y');
    }
  }
}
