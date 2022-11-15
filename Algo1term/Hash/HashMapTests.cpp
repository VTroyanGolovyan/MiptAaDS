#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "HashMap.hpp"

#include <string>
#include <vector>

TEST_CASE("InsertTests") {
  HashMap<std::string, int> ht;

  SECTION("Insert Lot and Find all, erase") {
   for (size_t i = 0; i < 100000; ++i) {
    ht.Insert(std::to_string(i), i);
   }
   size_t found_cnt = 0;
   for (size_t i = 0; i < 100000; ++i) {
    found_cnt += static_cast<size_t>(ht.Exists(std::to_string(i)));
   }
   REQUIRE(found_cnt == 100000);
   for (size_t i = 0; i < 100000; i+=5) {
    ht.Erase(std::to_string(i));
   }
   found_cnt = 0;
   for (size_t i = 0; i < 100000; ++i) {
    found_cnt += static_cast<size_t>(ht.Exists(std::to_string(i)));
   }
   //REQUIRE(found_cnt == 100000 - 100000/5);
  }
}

TEST_CASE("Operator []") {
  HashMap<std::string, std::string> ht;
  ht["Vlad"] = "Troian-Holovian";
  ht["English"] = "pain";
  SECTION("Check Ok") {
   REQUIRE(ht["Vlad"] == "Troian-Holovian");
   REQUIRE(ht["English"] == "pain");
  }
}
