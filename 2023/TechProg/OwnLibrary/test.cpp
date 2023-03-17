#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include "calc.hpp"

TEST_CASE("a + b works!") {
  SECTION("first test") {
    REQUIRE(foo(2,3) == 5);
  }
  SECTION("second test") {
    REQUIRE(foo(2,3) == 5);
  }
  
}