#include <iostream>
#include <vector>
#include <algorithm>

enum {
    White,
    Grey,
    Black
};

class OrientedGraph {
public:
    OrientedGraph() = default;

    OrientedGraph(const OrientedGraph& other) : graph_(other.graph_), n_(other.n_) {
    }

    OrientedGraph& operator=(const OrientedGraph& other) {
        if (std::addressof(other) == this) {
            return *this;
        }
        n_ = other.n_;
        graph_ = other.graph_;
        return *this;
    }

    OrientedGraph(OrientedGraph&& other) noexcept : graph_(std::move(other.graph_)), n_(other.n_) {
    }

    OrientedGraph& operator=(OrientedGraph&& other) noexcept {
        if (std::addressof(other) == this) {
            return *this;
        }
        n_ = other.n_;
        graph_ = std::move(other.graph_);
        return *this;
    }

    friend std::istream& operator>>(std::istream& in, OrientedGraph& graph) {
        in >> graph.n_;
        size_t m;
        in >> m;
        graph.graph_.assign(graph.n_, std::vector<size_t>());
        for (size_t i = 0; i < m; ++i) {
            size_t from, to;
            in >> from >> to;
            graph.graph_[from].push_back(to);
        }
        return in;
    }

    std::vector<size_t>& operator[](size_t i) {
        return graph_[i];
    }

    size_t VertexesCount() {
        return n_;
    }

    void AssignEmpty(size_t n) {
        n_ = n;
        graph_.assign(n, std::vector<size_t>());
    }

    void AddEdge(size_t from, size_t to) {
        graph_[from].push_back(to);
    }

    OrientedGraph Inverse() {
        OrientedGraph result;
        result.AssignEmpty(VertexesCount());
        size_t index = 0;
        for (auto&& adj : graph_) {
            for (auto&& to : adj) {
                result.AddEdge(to, index);
            }
            ++index;
        }
        return result;
    }

private:
    size_t n_{0};
    std::vector<std::vector<size_t>> graph_;
};

void dfs(
    OrientedGraph& g, // наш граф
    std::vector<char>& colors, // цвета
    std::vector<size_t>& top,   // массив для ответа
    size_t start,  // начало обхода
    size_t prev // предыдущая вершина
) {
    colors[start] = Grey;
    for (auto vertex : g[start]) {
        if (vertex != prev && colors[vertex] == White) {
            dfs(g, colors, top, vertex, start);
        }
    }
    top.emplace_back(start);
    colors[start] = Black;
}


void DfsComponent(
    OrientedGraph& g, // наш граф
    std::vector<char>& colors, // цвета
    std::vector<size_t>& component,   // массив для ответа
    size_t start,  // начало обхода
    size_t prev // предыдущая вершина
) {
    colors[start] = Grey;
    for (auto vertex : g[start]) {
        if (vertex != prev && colors[vertex] == White) {
            dfs(g, colors, component, vertex, start);
        }
    }
    component.emplace_back(start);
    colors[start] = Black;
}

std::vector<std::vector<size_t>> FindComponentsInInverted(
    OrientedGraph& g,
    std::vector<size_t>& order
) {
    std::vector<std::vector<size_t>> answer;
    std::vector<char> colors(g.VertexesCount(), White);
    for (auto&& vertex : order) {
        if (colors[vertex] == White) {
            std::vector<size_t> component;
            DfsComponent(g, colors, component, vertex, vertex);
            answer.emplace_back(std::move(component));
        }

    }
    return answer;
}

std::vector<size_t> TopSort(OrientedGraph& g) {
    std::vector<size_t> vertexes;
    std::vector<char> colors(g.VertexesCount(), White);
    vertexes.reserve(g.VertexesCount());
    for (size_t i = 0; i < g.VertexesCount(); ++i) {
        if (colors[i] == White) {
            dfs(g, colors, vertexes, i, i);
        }
    }
    std::reverse(vertexes.begin(), vertexes.end());
    return vertexes;
}

std::vector<std::vector<size_t>> FindComponents(OrientedGraph& g) {
    auto top_order = TopSort(g);
    auto inverted = g.Inverse();
    return FindComponentsInInverted(inverted, top_order);
}

int main() {
    OrientedGraph g;
    std::cin >> g;
    auto components = FindComponents(g);
    for (auto&& component : components) {
        for (auto&& vertex : component) {
            std::cout << vertex << " ";
        }
        std::cout << "\n";
    }
    return 0;
}
