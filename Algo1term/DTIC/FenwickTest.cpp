#define CATCH_CONFIG_MAIN
#include <iostream>

#include "Fenwick.hpp"
#include "catch.hpp"

TEST_CASE("Test Fenwick") {
  std::vector<size_t> x = {0, 1, 2, 3, 4, 5, 6, 7};
  Fenwick<size_t> t(x);

  SECTION("Test Sum") {
    REQUIRE(t.Sum(1, 3) == 6);
    REQUIRE(t.Sum(2, 3) == 5);
    REQUIRE(t.Sum(0, 7) == 28);
  }

  SECTION("Test Add") {
    t.Add(5, 100);
    REQUIRE(t.Sum(0, 7) == 128);
    for (int i = 0; i < 8; ++i) {
      t.Add(i, 1);
    }
    REQUIRE(t.Sum(0, 7) == 136);
  }
}
