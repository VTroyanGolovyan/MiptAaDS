#ifndef Sparse
#define Sparse

#include <vector>
#include <iostream>

template <typename T, typename OperationFunctor>
class SparseTable {
  public:
    SparseTable(const std::vector<T>& data)
      : logs_(data.size() + 1, 0),
        n_(data.size()) {
        PrecalcLogs(data.size() + 2);
        log_n_ = logs_[n_];
        sparse_.assign(log_n_ + 1, std::vector<T>(n_ + 1, T()));
        BuildSparse(data);
    }

    T Query(size_t l, size_t r) { // interval [l, r)
      size_t lg = logs_[r - l];
      return op_(sparse_[lg][l], sparse_[lg][r - (1 << lg)]);
    }

  private:
    void BuildSparse(const std::vector<T>& data) {
      sparse_[0] = data;
      for (size_t k = 1; k <= log_n_; ++k) {
        for (size_t i = 0; i + (1 << k) <= n_; ++i) {
          sparse_[k][i] = op_(sparse_[k - 1][i], sparse_[k - 1][i + (1 << (k - 1))]);
        }
      }
    }

    void PrecalcLogs(size_t n) {
      logs_[0] = 0;
      logs_[1] = 0;
      for (size_t i = 2; i <= n; ++i) {
        logs_[i] = logs_[i / 2]  + 1;
      }
    }

    size_t n_;
    size_t log_n_;
    std::vector<size_t> logs_;
    std::vector<std::vector<T>> sparse_;
    OperationFunctor op_;
};

#endif
