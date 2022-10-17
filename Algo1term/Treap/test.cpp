#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "treap.hpp"

TEST_CASE("sum") {
  srand(time(nullptr));
  TreapSet tp;
  SECTION("Find/Insert section") {
    for (int i = 0; i < 10000000; ++i) {
      tp.Insert(i);
    }
    for (int i = 1000000; i < 2000000; ++i) {
      tp.Erase(i);
    }
    for (int i = 2000000; i < 3000000; ++i) {
      tp.EraseFind(i);
    }
    REQUIRE(tp.Find(500000));
    REQUIRE(!tp.Find(1500000));
    REQUIRE(!tp.Find(2500000));

    int sum = 0;
    for (int i = 0; i < 10000000; ++i) {
      sum += tp.Find(i);
    }
    REQUIRE(sum == 8000000);
    for (int i = 10000000; i < 12000000; ++i) {
      tp.Insert(i);
    }
    sum = 0;
    for (int i = 0; i < 12000000; ++i) {
      sum += tp.Find(i);
    }
    REQUIRE(sum == 10000000);
  }

}
