#define CATCH_CONFIG_MAIN

#include "TreapWithImplicitKey.hpp"
#include "catch.hpp"

#include <iostream>
#include <string>

TEST_CASE("Test Tree") {
  TreapArray<int> arr;
  arr.Insert(0, 1);
  arr.Insert(1,10);

  SECTION("Test insert") {
    REQUIRE(arr[1] == 10);
    REQUIRE(arr[0] == 1);
    arr[0] = 100;
    REQUIRE(arr[0] == 100);
  }

  SECTION("Lot insert") {
    for (size_t i = 2; i < 100; ++i) {
      arr.Insert(i, i);
    }
    for (size_t i = 2; i < 100; ++i) {
      REQUIRE(arr[i] == i);
    }

    arr.Erase(50);
    for (size_t i = 50; i < 99; ++i) {
      REQUIRE(arr[i] == i + 1);
    }
  }
  
}

TEST_CASE("Stress Test Tree with implicit key") {
  TreapArray<std::string> arr;
  for (size_t i = 0; i < 1000000; ++i) {
    arr.Insert(i, std::to_string(i));
  }
  REQUIRE(arr.Size() == 1000000);
  REQUIRE(arr[99999] == "99999");
  arr.Erase(100000);
  REQUIRE(arr[100000] == "100001");
  REQUIRE(arr[800000] == "800001");
  REQUIRE(arr[99999] == "99999");
  for (size_t i = 0; i < 99999; ++i) {
    arr.Erase(100000);
  }
  REQUIRE(arr[100000] == "200000");
  REQUIRE(arr[99999] == "99999");
  REQUIRE(arr.Size() == 900000);
}

TEST_CASE("Stress Test Tree with implicit key MEGA TEST") {
  TreapArray<int64_t> arr;
  for (int64_t i = 0; i < 10000000; ++i) {
    arr.Insert(i, i);
  }

  for (int64_t i = 0; i < 10000000 - 1; ++i) {
    arr.Erase(0);
  }
  REQUIRE(arr.Size() == 1);
  REQUIRE(arr[0] == 9999999);
  arr.Erase(0);
  REQUIRE(arr.Size() == 0);
  arr.Insert(0, 1);
  REQUIRE(arr[0] == 1);
  REQUIRE(arr.Size() == 1);
}