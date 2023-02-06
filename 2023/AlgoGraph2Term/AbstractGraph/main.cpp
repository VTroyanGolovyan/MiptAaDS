#include <vector>
#include <iostream>
#include <unordered_map>
#include <numeric> 

template <class T>
using FilterFunction = std::function<bool(const T&)>;

template <class VType = size_t, class EType = std::pair<VType, VType>>
class IteratorImpl {
  public:
    using iterator = IteratorImpl;
     
    IteratorImpl(
      VType v,
      typename std::vector<VType>::iterator begin, 
      typename std::vector<VType>::iterator end, 
      const FilterFunction<EType>& filter
    ) : v_(v), now_(begin), end_(end), filter_(filter) {
      if (now_ != end_) {
        now_edge_ = {v_, *now_};
        if (!filter_(now_edge_)) { ++*this; }
      }
    }
    
    IteratorImpl& operator++() {
      do {
        ++now_;
        if (now_ == end_) {
          return *this;
        }
        now_edge_ = {v_, *now_};
      } while (!filter_(now_edge_) && now_ != end_);
      return *this;
    }
    
    bool operator==(const IteratorImpl& other) const {
      return now_ == other.now_;
    }

    bool operator!=(const IteratorImpl& other) const {
      return now_ != other.now_;
    }

    IteratorImpl begin() const {
      return *this;
    }

    IteratorImpl end() const {
      return {v_, end_, end_, filter_};
    }

    const EType& operator*() {
      return now_edge_;
    }

  private:
    EType now_edge_;
    VType v_;
    typename std::vector<VType>::iterator now_;
    typename std::vector<VType>::iterator end_;
    const FilterFunction<EType>& filter_;
};

template <class VType = size_t, class EType = std::pair<VType, VType>>
class Graph {
public:
  using VertexType = VType;
  using EdgeType = EType;

  virtual size_t EdgeCount() = 0;
  virtual size_t VertexCount() = 0;
  virtual std::vector<VType> Vertexes() = 0;

  virtual typename std::vector<VType>::iterator NeighboursBegin(VType v) = 0;
  virtual typename std::vector<VType>::iterator NeighboursEnd(VType v) = 0;
  virtual IteratorImpl<VType, EType>  NeighboursIt(
    VType v, 
    const FilterFunction<EType>& filter) = 0;

  virtual ~Graph() = default;
  
};

template <class VType = size_t, class EType = std::pair<VType, VType>>
class MatrixGraph : public Graph<VType, EType> {
  private: 
    // YOUR PAIN, MY FRIEND!      :)
    // HAHAHAHAHAHAHAHAHAHAHAAHAHAHA
    // UAHAHAHAHAHAHAHAHAHHAHHAHAHAH
    // IF WANT TO LIVE TRY TO DO IT!
};

template <class VType = size_t, class EType = std::pair<VType, VType>>
class ListGraph : public Graph<VType, EType> {
  public:
    ListGraph(
      const std::vector<VType>& vertexes, 
      const std::vector<EType>& edges
    ) : vertexes_(vertexes), num_vertex_(vertexes.size()), num_edges_(edges.size()) {
      for (auto& vertex : vertexes) {
        adjacency_lists_[vertex] = std::vector<VType>();
      }
      for (auto& edge : edges) {
        adjacency_lists_[edge.first].push_back(edge.second);
      }
    }

    size_t EdgeCount() override {
      return num_edges_;
    }

    size_t VertexCount() override {
      return num_edges_;
    }

    typename std::vector<VType>::iterator NeighboursBegin(VType v) override {
      return adjacency_lists_[v].begin();
    }

    typename std::vector<VType>::iterator NeighboursEnd(VType v) override {
      return adjacency_lists_[v].end();
    }

    std::vector<VType> Vertexes() override {
      return vertexes_;
    }
    
    IteratorImpl<VType, EType> NeighboursIt(
      VType v, 
      const FilterFunction<EType>& filter
    ) override {
      return {v, NeighboursBegin(v), NeighboursEnd(v), filter};
    }

  protected:
    std::unordered_map<VType, std::vector<VType> > adjacency_lists_;
    std::vector<VType> vertexes_;
    size_t num_vertex_;
    size_t num_edges_;
};

template <class VType = size_t, class EType = std::pair<VType, VType>> 
class UndirectedListGraph : public ListGraph<VType, EType> {
  public:
    using ListGraph<VType, EType>::adjacency_lists_;
    UndirectedListGraph(
      const std::vector<VType>& vertexes, 
      const std::vector<EType>& edges
    ) : ListGraph<VType, EType>(vertexes, edges) {
      for (auto& edge : edges) {
        adjacency_lists_[edge.second].push_back(edge.first);
      }
    }
};

enum Colors {
  white, 
  grey, 
  black
};

template <typename Graph>
void DFS(
  typename Graph::VertexType from, 
  Graph& graph, 
  std::unordered_map<typename Graph::VertexType, char>& colors
) {
  using VType = typename Graph::VertexType;
  using EType = typename Graph::EdgeType;
  colors[from] = grey;
  for (auto edge : graph.NeighboursIt(from, [&colors](const EType& edge) {
    return colors[edge.second] == white;
  })) {
    DFS(edge.second, graph, colors);
  }
  colors[from] = black;
}

template <typename Graph>
size_t CountOfComponents(Graph& graph) {
  using VType = typename Graph::VertexType;
  using Color = char;
  std::unordered_map<VType, Color> colors;
  auto vertexes = graph.Vertexes();
  std::for_each(vertexes.begin(), vertexes.end(), 
          [&](const VType& v) { colors[v] = white; });
  return std::accumulate(vertexes.begin(), vertexes.end(), 0ull, 
         [&colors, &graph](size_t cnt, VType v) {
         return (colors[v] == white ? (DFS(v, graph, colors), 1) : 0) + cnt;
    });
}


int main() {
  ListGraph<size_t> g({0,1,2,3,4}, {{0,1}, {1,2}, {3,4}, {0, 2}, {0, 7}});
  using EType = typename ListGraph<size_t>::EdgeType;
  for (auto edge : g.NeighboursIt(0, [](const EType& edge) { return true;})) {
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
