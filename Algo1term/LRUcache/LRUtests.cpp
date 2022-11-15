#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "LRU.hpp"

TEST_CASE("One Argument Lamda") {
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

TEST_CASE("Two Argument Lambda") {
  using binom_type = std::function<size_t(size_t, size_t)>;
  binom_type binom = [&binom](size_t n, size_t k) -> size_t {
    if (n == k || k == 0)
        return 1;
    return binom(n - 1, k) + binom(n - 1, k - 1);
  };

  LRUCache<1000, binom_type, size_t, size_t> bin_func(binom);
  binom = [&bin_func] (size_t n, size_t k) {
    return bin_func(n, k);
  };

  SECTION("C(10, 5)") {
    REQUIRE(binom(10, 5) == 252);
  }

  SECTION("C(160, 5)") {
    REQUIRE(binom(160, 5) == 820384032);
  }

}
