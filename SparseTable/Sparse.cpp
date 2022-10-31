#include "Sparse.hpp"
#include <iostream>

template<typename T>
class MinFunctor {
public:
  T operator()(const T& a, const T& b) {
    return a < b ? a : b;
  }
};

int main() {
  std::vector<int> data = {0,10,5,3,4,5};
  SparseTable<int, MinFunctor<int>> sparse(data);

  std::cout << sparse.Query(0, 6);
}
