#include <queue>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <numeric>

/* Own hash! */
template<>
struct std::hash<std::pair<size_t, size_t>> {
    std::size_t operator()(std::pair<size_t, size_t> const& s) const noexcept {
        auto hasher = std::hash<size_t> {}; 
        size_t hash =  hasher(s.first) + hasher(s.second);
        return hash;
    }
};

// класс абстрактной метрики
template <typename T>
class AbstractMetric {
public:
  // https://en.cppreference.com/w/cpp/language/operators тут есть примеры
  // As member function => why not use virtual?
  virtual T operator()(size_t from, size_t to) noexcept = 0;
  // virtual destructor - если забыли зачем welcome:
  // https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
  virtual ~AbstractMetric() = default;
};


// просто метрика заданая вручную весами ребер:)
template <typename T>
class Metric : public AbstractMetric<T> {
  public:
    Metric() {}

    void SetDist(size_t from, size_t to, T dist) {
      dist_[{std::min(from, to), std::max(from, to)}] = dist;
    }

    T operator()(size_t from, size_t to) noexcept override {
      return dist_[{std::min(from, to), std::max(from, to)}];
    }

  private:
    std::unordered_map<std::pair<size_t, size_t>, T> dist_;

};


/* why not, you really can do it!
template <typename T>
class EuclidianMetric : public AbstractMetric<T> {
  public:
    Metric() {}

    T operator()(size_t from, size_t to) noexcept override {
      // to do
    }

  private:
    // to do

}; */

template <typename T>
std::vector<std::pair<size_t, size_t>> FindNearestWay(
  const std::vector<std::vector<size_t>>& graph,
  AbstractMetric<T>& metric,
  size_t from, 
  size_t to
) {
  // массив distances 
  std::vector<T> distances(graph.size(), std::numeric_limits<T>::max());
  // массив предков - для восстановления пути
  std::vector<T> parents(graph.size(), std::numeric_limits<T>::max());
  distances[from] = T(); //  

  // std::priotity queue - просто бинарная куча(проходили в пред семестре)
  std::priority_queue<
    std::pair<T, size_t>, // элементы
    std::vector<std::pair<T, size_t>>, // базовый контейнер
    std::greater<std::pair<T, size_t>> // компаратор
  > queue; 
    
  // https://en.cppreference.com/w/cpp/container/priority_queue/emplace 
  queue.emplace(distances[from], from); // construct pairs in place 
  // что почитать, для преисполнения?
  // 0) L/Rvalue refs, Colapsing, !Forwarding references!
  //    https://en.cppreference.com/w/cpp/language/reference
  // 1) https://en.cppreference.com/w/cpp/utility/forward
  // 2) https://en.cppreference.com/w/cpp/utility/move 
  // что делать если это скучно?
  // 0) https://en.cppreference.com/w/cpp/language/value_category

  while (!queue.empty()) {
     auto [dist, now_near] = queue.top(); // до кого текущее расстояние минимально?
     queue.pop(); 
     if (dist > distances[now_near]) {  
      continue;
     }
     for (auto& neighbour : graph[now_near]) {
      T distance = metric(now_near, neighbour);
      // пробуем улучшить ответ
      if (distances[now_near] + distance < distances[neighbour]) {
        distances[neighbour] = distances[now_near] + distance;
        parents[neighbour] = now_near; // обновимся как текущий предок:)
        queue.emplace(distances[neighbour], neighbour); // добавим ка эту штуку в кучу, вдруг теперь это минимум:)
      }
     }
  }
  std::vector<std::pair<size_t, size_t>> path;
  size_t now = to;
  while (now != from) { // тупо идем до старта)
    path.emplace_back(parents[now], now); // construct in place)
    now = parents[now];
  }
  std::reverse(path.begin(), path.end()); // просто реверс
  return path;
}

/* пример
5 5
0 1 10
0 2 15
0 3 20
1 4 5
2 4 3
От 0 до 4 кратчайший путь 15 */

int main() {
  size_t vertexes, edges;
  std::cin >> vertexes >> edges;
  // для i ой вершины храним список соседей
  std::vector<std::vector<size_t>> graph(vertexes, std::vector<size_t>());

  Metric<size_t> dist;

  for (size_t i = 0; i < edges; ++i) {
    size_t from, to, weight;
    std::cin >> from >> to >> weight;
    graph[from].push_back(to);
    graph[to].push_back(from);
    dist.SetDist(from, to, weight);
  }

  auto path = FindNearestWay(graph, dist, 0, 4);
  std::cout << "Result way\n";
  for (auto [from, to] : path) {
    std::cout << from << " " << to << "\n";
  }

  /* пример использования std::accumulate */
  std::cout << "Way size\n";
  std::cout << std::accumulate(
    path.begin(), path.end(), 0ull, [&dist](unsigned long long acc, const std::pair<size_t, size_t>& edge) {
      return acc + dist(edge.first, edge.second);
    }
  ) << "\n";

  /* пример использования std::accumulate */
  // docs https://en.cppreference.com/w/cpp/algorithm/accumulate
  std::cout << "Accumulate example matrix sum\n";
  std::cout << std::accumulate(
    graph.begin(), graph.end(), 0ull, [&dist](unsigned long long acc, const std::vector<size_t>& line) {
      return acc + 
        std::accumulate(
           line.begin(), line.end(), 0ull, [&dist](unsigned long long acc, size_t x) {
              return acc + x;
        });
    }
  ) << "\n";

  return 0;

}
