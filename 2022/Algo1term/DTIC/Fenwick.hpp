#pragma once
#include <vector>

template <typename T>
class Fenwick {
 public:
  Fenwick(int64_t size) : data_(size, 0) {}

  Fenwick(const std::vector<T>& data) : data_(data.size(), 0) {
    for (int64_t i = 0; i < data.size(); ++i) {
      Add(i, data[i]);
    }
  }

  T Sum(int64_t l, int64_t r) {
    if (l == 0) {
      return Sum(r);
    }
    return Sum(r) - Sum(l - 1);
  }

  void Add(int64_t pos, const T& delta) {
    for (int64_t i = pos; i < data_.size(); i |= i + 1) {
      data_[i] += delta;
    }
  }

 private:
  T Sum(int64_t pos) {
    T sum = 0;
    for (int64_t i = pos; i >= 0; i = (i & (i + 1)) - 1) {
      sum += data_[i];
    }
    return sum;
  }

  std::vector<T> data_;
};