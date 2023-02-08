#include <iostream>
#include <functional>

#include "LRU.hpp"


int main() {
  using fib_type = std::function<size_t(size_t)>;
  fib_type fib = [&fib](size_t n) {
    if (n <= 1)
        return n;
    return fib(n - 2) + fib(n - 1);
  };
 
  LRUCache<1000, fib_type, size_t> func(fib);
  
  fib = [&func] (size_t n) {
    return func(n);
  };
  std::cout << func(155) << std::endl;

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
  std::cout << bin_func(20, 5) << std::endl;
}