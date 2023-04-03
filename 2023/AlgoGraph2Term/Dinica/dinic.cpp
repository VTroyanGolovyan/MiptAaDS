#include <algorithm>
#include <vector>
#include <queue>
#include <limits>
#include <iostream>

static constexpr int kInfinity = std::numeric_limits<int>::max();

enum {
  white,
  grey,
  black
};

struct Edge {
  int from;
  int to;
  int flow;
  int capacity;

  Edge(int from, int to, int capacity) 
    : from(from), to(to), flow(0), capacity(capacity) {}

  Edge(int from, int to, int flow, int capacity) 
    : from(from), to(to), flow(flow), capacity(capacity) {}
};

class Graph {
public:
  Graph(size_t n) : graph_(n, std::vector<size_t> ()) {
  }

  void AddEdge(int from, int to, int capacity) {
    edges_.emplace_back(from, to, capacity);
    size_t edge_id = edges_.size() - 1;
    edges_.emplace_back(to, from, 0);
    size_t back_edge_id = edges_.size() - 1;
    graph_[from].push_back(edge_id);
    graph_[to].push_back(back_edge_id);
  }

  bool BFS(size_t start, size_t end) {
    dist.assign(graph_.size(), kInfinity);
    std::queue<size_t> queue;
    dist[start] = 0;
    queue.push(start);
    while (!queue.empty() && dist[end] == kInfinity) {
      size_t vertex = queue.front();
      queue.pop(); 
      for (size_t i = 0; i < graph_[vertex].size(); ++i) {
        size_t id = graph_[vertex][i];
        if (dist[edges_[id].to] == kInfinity && edges_[id].flow < edges_[id].capacity) {
          queue.push(edges_[id].to);
          dist[edges_[id].to] = dist[edges_[id].from] + 1; 
        }
      }
    }
    return dist[end] != kInfinity; // увы ниче не нашли!
  }

  int DFS(std::vector<char>& colors, size_t vertex, size_t end, int current_flow) {
    if (current_flow <= 0) {
      return 0;
    }
    if (vertex == end) {
      return current_flow;
    }
    colors[vertex] = grey;
    for (size_t to_id : graph_[vertex]) {
      size_t to = edges_[to_id].to;
      if (colors[to] == white && dist[to] == dist[vertex] + 1) {
        int flow = DFS(
          colors,
          to, 
          end,
          std::min(current_flow, edges_[to_id].capacity - edges_[to_id].flow)
        );
        if (flow > 0) {
          edges_[to_id].flow += flow;
          edges_[to_id^1].flow -= flow;
          colors[vertex] = black;
          return flow;
        }
      }
    }

    colors[vertex] = black;
    return 0;
  }


  int DinicAlgo(size_t start, size_t end) {
    int flow = 0;
    while (BFS(start, end)) {
      int delta = 0;
      do {
        std::vector<char> colors(graph_.size(), white);
        delta = DFS(colors, start, end, kInfinity);
        flow += delta;
      } while(delta > 0);
    }
    return flow;
  }

private:
  std::vector<Edge> edges_;
  std::vector<std::vector<size_t>> graph_;
  std::vector<int> dist;
};

int main() {
 
  int n, m;
  std::cin >> n >> m;
  Graph gr(n);
  for (int i = 0; i < m; ++i) {
    int from, to, capacity;
    std::cin >> from >> to >> capacity;
    --from;
    --to;
    gr.AddEdge(from, to, capacity);
  }

  std::cout << gr.DinicAlgo(0, n - 1);

}

/*
4 5
0 1 1
0 2 3
2 1 2
2 3 1
1 3 2
*/
