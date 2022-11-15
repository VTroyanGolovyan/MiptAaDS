#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "LRU.hpp"

TEST_CASE("OneArgumentLamda") {
  std::vector<int> data = {-1};
  using fib_type = std::function<size_t(size_t)>;
  fib_type fib = [&fib](size_t n) {
    if (n <= 1)
        return n;
    return fib(n - 2) + fib(n - 1);
  };
 
  LRUCache<64, fib_type, size_t> func(fib);
  
  fib = [&func] (size_t n) {
    return func(n);
  };

  SECTION("fib(3)") {
    REQUIRE(fib(3) == 2);
  }
  
  SECTION("fib(4)") {
    REQUIRE(fib(4) == 3);
  }

  SECTION("Seauence test(2 - 1000") {
    size_t a = 0;
    size_t b = 1;
    for (int i = 2; i <= 1000; ++i) {
      size_t c = a + b;
      REQUIRE(fib(i) == c);
      a = b;
      b = c;
    }
  }

}
