#include <iostream>
#include <vector>
#include <limits>

static constexpr int64_t kInfinity = std::numeric_limits<int64_t>::max();

struct Edge {
  int64_t to_vertex; // куда идет ребро
  int64_t flow; // поток по ребрышку
  int64_t capacity; // пропускная способность

  int64_t CurrentCapacity() {
    return capacity - flow;
  }

  Edge(int64_t to_vertex, int64_t capacity) : to_vertex(to_vertex), capacity(capacity), flow(0) {
  }
};

void AddEdge(std::vector<std::vector<int64_t>>& graph, std::vector<Edge>& edges, int64_t from, int64_t to, int64_t capacity) {
  edges.emplace_back(to, capacity);
  graph[from].emplace_back(edges.size() - 1);
  edges.emplace_back(from, 0); // back edge capacity = 0
  graph[to].emplace_back(edges.size() - 1);
}

int64_t FindIncreasingPath(
  std::vector<std::vector<int64_t>>& graph, 
  std::vector<Edge>& edges, 
  std::vector<size_t>& used,
  size_t phase,
  int64_t v,
  int64_t t,
  int64_t min
) {
  if (t == v) {
    std::cout << "step: " << t << " " << v << std::endl;
    return min;
  }
  used[v] = phase;
  for (int64_t edge_id : graph[v]) {
    std::cout << " " << edge_id << std::endl;
    if (edges[edge_id].CurrentCapacity() == 0) {
      std::cout << edge_id << std::endl;
      continue;
    }
    if (used[edges[edge_id].to_vertex] == phase) {
      std::cout << edge_id << std::endl;
      continue;
    }
    int64_t delta = FindIncreasingPath(
      graph, 
      edges, 
      used, 
      phase, 
      edges[edge_id].to_vertex, 
      t, 
      std::min(min, edges[edge_id].CurrentCapacity())
    );
    if (delta > 0) {
      std::cout << delta << std::endl;
      edges[edge_id].flow += delta;
      edges[edge_id ^ 1].flow -= delta; // back edge
      return delta;
    }
  }
  return 0;
}

int64_t MaxFlow(
  std::vector<std::vector<int64_t>>& graph, 
  std::vector<Edge>& edges, 
  int64_t s, 
  int64_t t
) {
  std::vector<size_t> used(graph.size(), 0);
  uint8_t phase = 1;
  while (FindIncreasingPath(graph, edges, used, phase, s, t, kInfinity)) {
    ++phase;
  }

  int64_t result = 0;
  for (int64_t i = 0; i < graph[s].size(); ++i) {
    result += edges[graph[s][i]].flow;
  }
  return result;
}

int main() {
  std::vector<Edge> edges;
  size_t n, m;
  std::cin >> n >> m;
  std::vector<std::vector<int64_t>> graph(n, std::vector<int64_t>());
  for (size_t i = 0; i < m; ++i) {
    int64_t from, to, capacity;
    std::cin >> from >> to >> capacity;
    AddEdge(graph, edges, from, to, capacity); // добавить ребро и обратное ребро
  }
  std::cout << std::endl;
  for (auto& to : graph) {
    for (auto& edge : to) {
      std::cout << edges[edge].to_vertex << " " << edges[edge].flow << " " << edges[edge].capacity << std::endl;
    }
  }
  std::cout << std::endl;
  int64_t s, t;
  std::cin >> s >> t;
 
  for (auto& to : graph) {
    for (auto& edge : to) {
      std::cout << edges[edge].to_vertex << " " << edges[edge].flow << " " << edges[edge].capacity << std::endl;
    }
  }
  std::cout << std::endl;


  std::cout << MaxFlow(graph, edges, s, t) << std::endl;
  return 0;
}
