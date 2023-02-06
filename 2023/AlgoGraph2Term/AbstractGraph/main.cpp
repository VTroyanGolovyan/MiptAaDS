#include <vector>
#include <iostream>
#include <unordered_map>
#include <stack>
#include <numeric> 

template <class VType = size_t, class EType = std::pair<VType, VType>>
class IteratorImpl {
  public:
    using FilterFunction = std::function<bool(const EType&)>;
    using iterator = IteratorImpl;

    IteratorImpl(
      VType v,
      typename std::vector<VType>::iterator begin, 
      typename std::vector<VType>::iterator end, 
      const FilterFunction& filter
    ) : v(v), now(begin), end_(end), filter(filter) {
      if (now != end) {
        now_edge_ = {v, *now};
        while (!filter(now_edge_) && now != end) {
          ++now;
          now_edge_ = {v, *now};
        }
      }
    }
    
    IteratorImpl& operator++() {
      if (now == end_) {
        return *this;
      }
      do {
        ++now;
        if (now == end_) return *this;
        now_edge_ = {v, *now};
      } while (!filter(now_edge_) && now != end_);
      return *this;
    }
    
    bool operator == (const IteratorImpl& other) const {
      return now == other.now;
    }

    bool operator != (const IteratorImpl& other) const {
      return now != other.now;
    }

    IteratorImpl begin() {
      return *this;
    }

    IteratorImpl end() {
      return IteratorImpl(v, end_, end_, filter);
    }

    const EType& operator* () {
      return now_edge_;
    }
  private:
    EType now_edge_;
    VType v;
    typename std::vector<VType>::iterator now;
    typename std::vector<VType>::iterator end_;
    const FilterFunction& filter;
};

template <class VType = size_t, class EType = std::pair<VType, VType>>
class Graph {
public:
  using FilterFunction = std::function<bool(const EType&)>;
  virtual size_t EdgeCount() = 0;
  virtual size_t VertexCount() = 0;
  virtual std::vector<VType> Vertexes() = 0;
  virtual typename std::vector<VType>::iterator Neighbours(VType v) = 0;
  virtual typename std::vector<VType>::iterator NeighboursEnd(VType v) = 0;
  virtual IteratorImpl<VType, EType>  NeighboursIt(
    VType v, 
    const FilterFunction& filter) = 0;
  virtual ~Graph() = default;
};

template <class VType = size_t, class EType = std::pair<VType, VType>>
class MatrixGraph : public Graph<VType, EType> {
  private: 
    // Its for you:)
};

template <class VType = size_t, class EType = std::pair<VType, VType>>
class ListGraph : public Graph<VType, EType> {
  public:
    using VertexType = VType;
    using FilterFunction = std::function<bool(const EType&)>;
    ListGraph(
      const std::vector<VType>& vertexes, 
      const std::vector<EType>& edges
    ) : verts_(vertexes), num_vertex_(vertexes.size()), num_edges_(edges.size()) {
      for (auto& vertex : vertexes) {
        vertex_[vertex] = std::vector<VType>();
      }
      for (auto& edge : edges) {
        vertex_[edge.first].push_back(edge.second);
      }
    }

    size_t EdgeCount() override {
      return num_edges_;
    }

    size_t VertexCount() override {
      return num_edges_;
    }

    typename std::vector<VType>::iterator Neighbours(VType v) override {
      return vertex_[v].begin();
    }

    typename std::vector<VType>::iterator NeighboursEnd(VType v) override {
      return vertex_[v].end();
    }

    std::vector<VType> Vertexes() override {
      return verts_;
    }
    IteratorImpl<VType, EType> NeighboursIt(
    VType v, 
    const FilterFunction& filter) override {
        return IteratorImpl(v, Neighbours(v), NeighboursEnd(v), filter);
    }
  protected:

    std::unordered_map<VType, std::vector<VType> > vertex_;
    std::vector<VType> verts_;

    size_t num_vertex_;
    size_t num_edges_;
};

enum Colors {
  white, grey, black
};

template <typename Graph>
void DFS(
  typename Graph::VertexType from, 
  Graph& graph, 
  std::unordered_map<typename Graph::VertexType, char>& colors) {

  using VType = typename Graph::VertexType;

  colors[from] = grey;

  for (auto edge : graph.NeighboursIt(from, [&colors](const std::pair<VType, VType>& edge) {
    return colors[edge.second] == white;
  })) {
    DFS(edge.second, graph, colors);
  }
  colors[from] = black;
}

template <class VType = size_t, class EType = std::pair<VType, VType>> 
class UndirectedListGraph : public ListGraph<VType, EType> {
  public:
    using ListGraph<VType, EType>::vertex_;
    UndirectedListGraph(
      const std::vector<VType>& vertexes, 
      const std::vector<EType>& edges) 
    : ListGraph<VType, EType>(vertexes, edges) {
        for (auto& edge : edges) {
          vertex_[edge.second].push_back(edge.first);
        }
      }
};

template <typename Graph>
size_t CountOfComponents(Graph& graph) {
  using VType = typename Graph::VertexType;
  std::unordered_map<VType, char> colors;
  auto vert = graph.Vertexes();
  for (auto v : vert) {
    colors[v] = white;
  }
  return std::accumulate(vert.begin(), vert.end(), 0uz, 
    [&colors, &graph](size_t cnt, VType v) {
      return (colors[v] == white ? (DFS(v, graph, colors), 1) : 0) + cnt;
    });
}


int main() {
  ListGraph<size_t> g({0,1,2,3,4}, {{0,1}, {1,2}, {3,4}, {0, 2}, {0, 7}});
  for (auto edge : g.NeighboursIt(0,
   [](const std::pair<size_t, size_t>& edge) {
    return true;
    })) {
      std::cout << edge.first << " " << edge.second << std::endl;
  }

  UndirectedListGraph<size_t> g2({0,1,2,3,4}, {{0,1}, {1,2}, {3,4}, {0, 2}});
  std::cout << CountOfComponents(g2) << std::endl; // must be 2

  UndirectedListGraph<size_t> g3({0,1,2,3,4,5,6,7}, {{0,1}, {1,2}, {3,4}, {0, 2}, {6,7}, {7, 6}});
  std::cout << CountOfComponents(g3) << std::endl; // must be 4

  UndirectedListGraph<std::string> g4(
    {"a","b","c","d","e"}, 
    {{"a","b"}, {"b","c"}, {"d","e"}, {"a", "c"}}
  );
  std::cout << CountOfComponents(g4) << std::endl; // must be 2

  return 0;
}
