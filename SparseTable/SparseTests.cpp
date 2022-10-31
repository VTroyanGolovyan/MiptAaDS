#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Sparse.hpp"

template<typename T>
class MinFunctor {
public:
  T operator()(const T& a, const T& b) {
    return a < b ? a : b;
  }
};

template<typename T>
class MaxFunctor {
public:
  T operator()(const T& a, const T& b) {
    return a > b ? a : b;
  }
};

template<typename T>
class BitAndFunctor {
public:
  T operator()(const T& a, const T& b) {
    return a & b;
  }
};

template<typename T>
class BitOrFunctor {
public:
  T operator()(const T& a, const T& b) {
    return a | b;
  }
};

TEST_CASE("SimpleSparseTestInt") {
  std::vector<int> data = {0,10,5,3,4,5};
  SparseTable<int, MinFunctor<int>> sparse(data);


  SECTION("[0, 1)") {
    REQUIRE(sparse.Query(0, 1) == 0);
  }
  SECTION("[1, 2)") {
    REQUIRE(sparse.Query(1, 2) == 10);
  }
  SECTION("[2, 3)") {
    REQUIRE(sparse.Query(2, 3) == 5);
  }
  SECTION("[3, 4)") {
    REQUIRE(sparse.Query(3, 4) == 3);
  }
  SECTION("[4, 5)") {
    REQUIRE(sparse.Query(4, 5) == 4);
  }
  SECTION("[5, 6)") {
    REQUIRE(sparse.Query(5, 6) == 5);
  }
  SECTION("[2, 6)") {
    REQUIRE(sparse.Query(2, 6) == 3);
  }
  SECTION("[2, 5)") {
    REQUIRE(sparse.Query(2, 5) == 3);
  }
  SECTION("[0, 6)") {
    REQUIRE(sparse.Query(0, 6) == 0);
  }
}

TEST_CASE("OneElementArray") {
  std::vector<int> data = {-1};
  SparseTable<int, MinFunctor<int>> sparse(data);

  SECTION("[0, 1)") {
    REQUIRE(sparse.Query(0, 1) == -1);
  }
}

TEST_CASE("Stress") {
  std::vector<int> data;
  for (size_t i = 0; i < 2000000; ++i) {
    data.push_back(i);
  }
  data[1250000] = -1;
  SparseTable<int, MinFunctor<int>> sparse(data);

  SECTION("[1000000, 1500000)") {
    REQUIRE(sparse.Query(1000000, 1500000) == -1);
  }
}

TEST_CASE("String test") {
  std::vector<std::string> data = {"aba", "caba", "a", "baba", "capa", "cappa"};
  SECTION("String test MinFunctor") {
    SparseTable<std::string, MinFunctor<std::string>> sparse(data);
    REQUIRE(sparse.Query(0, 4) == "a");
    REQUIRE(sparse.Query(0, data.size()) == "a");
    REQUIRE(sparse.Query(2, 3) == "a");
    REQUIRE(sparse.Query(4, 6) == "capa");
  }
  SECTION("String test MaxFunctor") {
    SparseTable<std::string, MaxFunctor<std::string>> sparse(data);
    REQUIRE(sparse.Query(0, 4) == "caba");
    REQUIRE(sparse.Query(0, data.size()) == "cappa");
    REQUIRE(sparse.Query(2, 3) == "a");
    REQUIRE(sparse.Query(4, 6) == "cappa");
  }
}

TEST_CASE("Bit test") {
  std::vector<size_t> data = {0, 1, 0, 1, 1, 1};
  SECTION("BitAnd test") {
    SparseTable<size_t, BitAndFunctor<size_t>> sparse(data);
    REQUIRE(sparse.Query(0, 4) == 0);
    REQUIRE(sparse.Query(0, data.size()) == 0);
    REQUIRE(sparse.Query(2, 3) == 0);
    REQUIRE(sparse.Query(4, 6) == 1);
  }
  SECTION("BitOr test") {
    SparseTable<size_t, BitOrFunctor<size_t>> sparse(data);
    REQUIRE(sparse.Query(0, 4) == 1);
    REQUIRE(sparse.Query(0, data.size()) == 1);
    REQUIRE(sparse.Query(2, 3) == 0);
    REQUIRE(sparse.Query(4, 6) == 1);
  }
}

TEST_CASE("SuperStress") {
  std::vector<size_t> data;
  for (size_t i = 0; i < 5000000; ++i) {
    data.push_back(i);
  }

  SECTION("Lot queries with min functor") {
    SparseTable<size_t, MinFunctor<size_t>> sparse(data);

    size_t interval_delta = 5;
    size_t real_sum = 0;
    size_t query_sum = 0;
    for (size_t i = 0; i < 5000000 - interval_delta; ++i) {
      real_sum += i;
      query_sum += sparse.Query(i, i + interval_delta);
    }
    REQUIRE(query_sum == real_sum);
  }

  SECTION("Lot queries with max functor and different delta") {
    SparseTable<size_t, MaxFunctor<size_t>> sparse(data);

    size_t interval_delta = 5;
    size_t real_sum = 0;
    size_t query_sum = 0;
    while (interval_delta < 100000) {
      for (size_t i = 0; i < 5000000 - interval_delta; ++i) {
        real_sum += data[i + interval_delta - 1];
        query_sum += sparse.Query(i, i + interval_delta);
      }
      interval_delta *= 25;
    }
    REQUIRE(query_sum == real_sum);
  }
}
