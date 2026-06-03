#include "../../code/data_structures/avl.cpp"

#include <string>
#include <vector>

#include "../doctest.h"

// Custom structure to test custom sorting logic criteria
struct CustomItem {
  int id;
  std::string name;

  bool operator==(const CustomItem& o) const { return id == o.id; }
  bool operator!=(const CustomItem& o) const { return id != o.id; }
  bool operator<(const CustomItem& o) const { return id < o.id; }
  bool operator>(const CustomItem& o) const { return id > o.id; }
  bool operator<=(const CustomItem& o) const { return id <= o.id; }
  bool operator>=(const CustomItem& o) const { return id >= o.id; }
};

TEST_SUITE("AVL Tree Suite") {
  TEST_CASE("Basic Tree Mechanics") {
    avl<int> tree;

    SUBCASE("Empty Tree Initial State") {
      CHECK(tree.in_order().empty() == true);
      CHECK(tree.find(42) == false);
      CHECK(tree.order_of_key(10) == 0);
    }

    SUBCASE("Unique Insertions and Searches") {
      tree.insert(5);
      tree.insert(15);
      tree.insert(5);  // Duplicate item entry testing (should be safely ignored)

      CHECK(tree.find(5) == true);
      CHECK(tree.find(15) == true);
      CHECK(tree.find(10) == false);
      CHECK(tree.in_order() == std::vector<int>{5, 15});
    }
  }

  TEST_CASE("Rebalancing Cascades (Rotations)") {
    SUBCASE("Right-Right Linear Chain -> Left Rotation") {
      avl<int> tree;
      tree.insert(1);
      tree.insert(2);
      tree.insert(3);  // Triggers rebalance sequence

      CHECK(tree.in_order() == std::vector<int>{1, 2, 3});
      CHECK(tree.kth(1, 0) == 2);  // Root must become median node 2
    }

    SUBCASE("Left-Right Zig-Zag Chain -> Double Rotation") {
      avl<int> tree;
      tree.insert(30);
      tree.insert(10);
      tree.insert(20);  // Triggers Left-Right shift balancing

      CHECK(tree.in_order() == std::vector<int>{10, 20, 30});
      CHECK(tree.kth(0, 0) == 10);
      CHECK(tree.kth(1, 0) == 20);
    }
  }

  TEST_CASE("Erase Operations") {
    avl<int> tree({10, 20, 30, 40, 50});

    SUBCASE("Removing Leaf Target Node") {
      tree.erase(50);
      CHECK(tree.find(50) == false);
      CHECK(tree.in_order() == std::vector<int>{10, 20, 30, 40});
    }

    SUBCASE("Removing Internal Node with Two Subtrees") {
      tree.erase(30);
      CHECK(tree.find(30) == false);
      CHECK(tree.in_order() == std::vector<int>{10, 20, 40, 50});
    }
  }

  TEST_CASE("Order Statistic Methods") {
    avl<int> tree({5, 1, 9, 3, 7});
    // Sorted structure layout: [1, 3, 5, 7, 9]

    SUBCASE("Verifying kth retrieval boundaries") {
      CHECK(tree.kth(0, 0) == 1);  // 0-indexed minimum entry
      CHECK(tree.kth(2, 0) == 5);  // Median position check
      CHECK(tree.kth(4, 0) == 9);  // Tail bound mapping

      CHECK(tree.kth(1, 1) == 1);  // 1-indexed option check
      CHECK(tree.kth(5, 1) == 9);
    }

    SUBCASE("Verifying order_of_key lookup indexes") {
      CHECK(tree.order_of_key(0) == 0);   // Element smaller than minimum entry
      CHECK(tree.order_of_key(1) == 0);   // Equal matching lower boundary
      CHECK(tree.order_of_key(4) == 2);   // Insertion index between 3 and 5
      CHECK(tree.order_of_key(10) == 5);  // Out-of-bounds upper limit index
    }
  }

  TEST_CASE("Custom Type Inclusions Evaluation") {
    avl<CustomItem> tree;
    tree.insert({100, "Alice"});
    tree.insert({50, "Bob"});

    CHECK(tree.find({50, ""}) == true);
    CHECK(tree.kth(0, 0).name == "Bob");
  }
}
