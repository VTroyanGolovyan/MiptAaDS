
#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include "a_plus_b.hpp"

TEST_CASE("Test a+b") {
  SECTION("first test") {
    REQUIRE(a_plus_b(2,3) == 5);
  }
  
  SECTION("second test") {
    REQUIRE(a_plus_b(2,3) == 5);
  }
 
  SECTION("first test") {
    REQUIRE(a_plus_b(2,3) == 5);
  }

}

TEST_CASE("Test a+b+c") {

  SECTION("first test") {
    REQUIRE(a_plus_b(2,3,3) == 8);
  }
  
  SECTION("second test") {
    REQUIRE(a_plus_b(2,3,2) == 7);
  }

  SECTION("first test") {
    REQUIRE(a_plus_b(2,4,3) == 9);
  }

}

