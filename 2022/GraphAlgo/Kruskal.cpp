#include <iostream>
#include <vector>
#include <algorithm>

class DST {
  public:
    DST(size_t n) : sets_(n), weights_(n, 1) {
      for (size_t i = 0; i < n; ++i) {
        sets_[i] = i;
      }
    }

    size_t GetSet(size_t v) {
      if (sets_[v] == v) {
        return v;
      }
      size_t result = GetSet(sets_[v]);
      sets_[v] = result ;
      return result;
    }

    void Union(size_t first, size_t second) {
      size_t a_set = GetSet(first);
      size_t b_set = GetSet(second);
      if (weights_[a_set] > weights_[b_set]) {
        std::swap(a_set, b_set);
      }
      weights_[b_set] += weights_[a_set];
      sets_[a_set] = b_set;
    }

  private:
    std::vector<size_t> sets_;
    std::vector<size_t> weights_;
};

struct Edge {
  size_t weight;
  size_t from;
  size_t to;
  Edge() = default;
  Edge& operator=(const Edge& other) = default;
  Edge(const Edge& other) = default;
  bool operator < (const Edge& second) const {
    return weight < second.weight;
  }
};

std::istream& operator>>(std::istream& in, Edge& edge) {
  in >> edge.weight >> edge.from >> edge.to;
  return in;
}


std::vector<Edge> MST(size_t n, std::vector<Edge>& edges) {
  std::sort(edges.begin(), edges.end());

  DST dst(n);
  std::vector<Edge> mst;

  for (auto&& edge : edges) {
    if (dst.GetSet(edge.from) != dst.GetSet(edge.to)) {
      dst.Union(edge.from, edge.to);
      mst.push_back(edge);
    }
  }
  return mst;
}

int main() {

  size_t n;
  size_t m;
  std::cin >> n >> m;
  std::vector<Edge> edges(m);
  for (auto&& edge : edges) {
    std::cin >> edge;
  }

  std::cout << std::endl;
  std::cout << std::endl;
  auto mst = MST(n, edges);
  std::cout << "MST: " << std::endl;
  for (auto&& edge : mst) {
    std::cout << edge.weight << " " << edge.from << " " << edge.to << std::endl;
  }
  return 0;
}
