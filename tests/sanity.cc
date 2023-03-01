
#include "catch2/catch.hpp"

// Some basic assertions.
TEST_CASE("Sanity", "[sanity]") {
    // Expect two strings not to be equal.
    REQUIRE("hello" != "world");

    // Expect equality
    REQUIRE(11 * 12 == 132);
}
