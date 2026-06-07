// =============================================================================
// @author     Jose A. Romero (jromero132)
// @unit_test  Randomized K-D Tree
// @docs       Comprehensive suite verifying multi-dimensional spatial
//             partitioning invariants, exact coordinate matching, nearest
//             neighbor pruning queries, and tree mutations across 2D and 3D.
// =============================================================================

#include <vector>

#include "../../code/data_structures/randomized_kd_tree.cpp"
#include "../doctest.h"

TEST_CASE("Randomized K-D Tree - Comprehensive Verification Suite") {
  SUBCASE("2D Tree: Empty Boundaries & Initial State") {
    randomized_kd_tree<2> tree;
    CHECK(tree.size() == 0);

    // Nearest neighbor query on an empty tree should fall back to a zeroed default point
    point<2> empty_query = {0.0, 0.0};
    point<2> default_res = tree.closest(empty_query);
    CHECK(default_res[0] == 0.0);
    CHECK(default_res[1] == 0.0);
  }

  SUBCASE("2D Tree: Static Balanced Build & Quadrant Queries") {
    randomized_kd_tree<2> tree;
    std::vector<point<2>> points = {{2.0, 3.0}, {5.0, 4.0}, {9.0, 6.0},
                                    {4.0, 7.0}, {8.0, 1.0}, {7.0, 2.0}};

    tree.build(points);
    REQUIRE(tree.size() == 6);

    // Verify lookup closest to target (9, 2)
    point<2> query1 = {9.0, 2.0};
    point<2> expected1 = {8.0, 1.0};
    CHECK(tree.closest(query1) == expected1);

    // Verify lookup closest to target (1, 2)
    point<2> query2 = {1.0, 2.0};
    point<2> expected2 = {2.0, 3.0};
    CHECK(tree.closest(query2) == expected2);
  }

  SUBCASE("2D Tree: Interleaved Dynamic Mutation & Deletion Hooks") {
    randomized_kd_tree<2> tree;
    std::vector<point<2>> points = {{2.0, 3.0}, {5.0, 4.0}, {9.0, 6.0},
                                    {4.0, 7.0}, {8.0, 1.0}, {7.0, 2.0}};
    tree.build(points);

    // Inject a dynamic point targeting the close neighborhood of query (1, 2)
    point<2> dynamic_insert = {1.0, 1.0};
    tree.insert(dynamic_insert);
    REQUIRE(tree.size() == 7);

    // Nearest neighbor must change from (2,3) to the newly inserted (1,1)
    point<2> query = {1.0, 2.0};
    CHECK(tree.closest(query) == dynamic_insert);

    // Remove the inserted point to evaluate re-balancing and recovery path
    tree.remove(dynamic_insert);
    REQUIRE(tree.size() == 6);

    // Target lookup should revert gracefully back to the baseline coordinate
    point<2> expected_revert = {2.0, 3.0};
    CHECK(tree.closest(query) == expected_revert);
  }

  SUBCASE("3D Tree: Higher Dimension Geometry and Cluster Lookups") {
    randomized_kd_tree<3> tree3d;
    std::vector<point<3>> points3d = {{1.0, 1.0, 1.0}, {10.0, 10.0, 10.0}, {5.0, 5.0, 5.0}};

    tree3d.build(points3d);
    REQUIRE(tree3d.size() == 3);

    // Coordinate verification on 3D Euclidean hyperspherical bounds
    point<3> query3d = {9.0, 8.0, 9.0};
    point<3> expected3d = {10.0, 10.0, 10.0};

    point<3> result3d = tree3d.closest(query3d);
    CHECK(result3d == expected3d);
    CHECK(result3d[0] == 10.0);
    CHECK(result3d[1] == 10.0);
    CHECK(result3d[2] == 10.0);
  }
}
