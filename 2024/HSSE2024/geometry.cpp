#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <iomanip>

template <int dim = 2, typename T = double>
class Point {
public:
    Point(std::initializer_list<T> from) {
        auto it = from.begin();
        for (int i = 0; i < dim; ++i) {
            coordinates_[i] = *it;
            ++it;
        }
    }

    template <typename RandIt>
    Point(RandIt from) : coordinates_(from, from + dim) {
    }

    Point() {
        std::fill(coordinates_.begin(), coordinates_.end(), T());
    }

    T Get(int axis) {
        return coordinates_[axis];
    }

    std::string ToString() const {
        std::string res;
        res += "(";
        for (const auto& x_i : coordinates_) {
            res += std::to_string(x_i) + " ";
        }
        if (!res.empty()) {
            res.pop_back();
        }
        res += ")";
        
        return res;
    }

    T SquaredAbs() const {
        T res = 0.;
        for (const auto& x_i : coordinates_) {
            res += x_i * x_i;
        }
        return res;
    }

    template <typename U> 
    Point<dim, U> CastTo() {
        return Point<dim, U>(coordinates_.begin());
    }

    Point operator-(const Point<dim, T>& other) const {
        Point<dim, T> res;
        for (size_t i = 0; i < dim; ++i) {
            res.coordinates_[i] = coordinates_[i] - other.coordinates_[i];
        }
        return res;
    }

    Point operator*(T multiplier) const {
        Point<dim, T> res;
        for (size_t i = 0; i < dim; ++i) {
            res.coordinates_[i] = coordinates_[i] * multiplier;
        }
        return res;
    }

    
    Point operator+(const Point<dim, T>& other) const {
        Point<dim, T> res;
        for (size_t i = 0; i < dim; ++i) {
            res.coordinates_[i] = coordinates_[i] + other.coordinates_[i];
        }
        return res;
    }

    friend std::ostream& operator<<(std::ostream& out, const Point<dim, T>& point) {
        
        out << point.ToString();
        return out;
    }
private:
    std::array<T, dim> coordinates_;
};

template <int dim = 2, typename T = double>
Point<dim, T> operator*(T multiplier, const Point<dim, T>& point) {
    return point * multiplier;
} 


template <int dim = 2, typename T = double>
using Vector = Point<dim, T>;

template <int dim = 2, typename T = double>
class Segment {
public:
    Segment(Point<dim, T> start, Point<dim, T> end) : start_(start), end_(end) {}

    Point<dim, T> GetStart() const {
        return start_;
    }

    Point<dim, T> GetEnd() const {
        return end_;
    }
private:
    Point<dim, T> start_;
    Point<dim, T> end_;
};

template <typename T, typename F>
T TernarySearch(T start, T end, T eps, F f) {
    while (end - start > eps) {
        T split_a = start + (end - start) / 3.;
        T split_b = end - (end - start) / 3.;
        if (f(split_a) > f(split_b)) {
            start = split_a;
        } else {
            end = split_b;
        }
    }
    return (start + end) / 2.;
}


template <int dim = 2, typename T = double>
T CalculateSqrDistance(const Point<dim, T>& point, const Segment<dim, T>& segment) {
    T k_min = 0.;
    T k_max = 1.;

    Vector<dim, T> vec = segment.GetEnd() - segment.GetStart();
    
    Vector<dim, T> r1 = segment.GetStart();

    auto dist = [&](T k) {
        return ((r1 + k * vec) - point).SquaredAbs();
    };

    T k = TernarySearch(k_min, k_max, 1e-9l, dist);

    return dist(k);
}

template <int dim = 2, typename T = double>
T CalculateSqrDistance(const Segment<dim, T>& first, const Segment<dim, T>& second) {
    T k_min = 0;
    T k_max = 1.;

    Vector<dim, T> vec1 = first.GetEnd() - first.GetStart();
    Vector<dim, T> r1 = first.GetStart();

    auto get_dot_dist = [&](T k) {
        return CalculateSqrDistance(r1 + k * vec1, second);
    };

    auto k = TernarySearch(
        k_min,
        k_max,
        1e-9l,
        get_dot_dist
    );

    return CalculateSqrDistance(r1 + k * vec1, second);
}

int main() {
    
    long double x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    Point<2, long double> p1 = {x1, y1};
    Point<2, long double> p2 = {x2, y2};

    std::cin >> x1 >> y1 >> x2 >> y2;
    Point<2, long double> p3 = {x1, y1};
    Point<2, long double> p4 = {x2, y2};
    Segment<2, long double> seg1(p1, p2);
    Segment<2, long double> seg2(p3, p4);

    std::cout << std::fixed << std::setprecision(6) << std::pow(CalculateSqrDistance(seg1, seg2), 0.5) << std::endl;

    return 0;
}

