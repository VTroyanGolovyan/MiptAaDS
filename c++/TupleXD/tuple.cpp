#include <utility>
#include <string>
#include <iostream>

template <typename... T>
class Tuple;

template <>
class Tuple<> {
  friend std::ostream& operator<<(std::ostream& out, const Tuple<>& tp) {
    return out;
  }

  friend std::istream& operator>>(std::istream& in, Tuple<>& tp) {
    return in;
  }

  bool operator == (const Tuple<>& other) {
    return true;
  }

  std::strong_ordering operator <=> (const Tuple<>& other) {
    return std::strong_ordering::equivalent;
  }

  static constexpr int size = 0;
};


template <typename Head, typename... Tail>
class Tuple<Head, Tail...> : Tuple<Tail...> {
public:

  Tuple(Head head, Tail... tail)
    : Tuple<Tail...>(tail...),
      head_(head), tail_(*this)  {

  }

  friend std::ostream& operator<<(std::ostream& out, const Tuple<Head, Tail...>& tp) {
    out << tp.head_ << " ";
    out << tp.tail_;
    return out;
  }

  friend std::istream& operator>>(std::istream& in, Tuple<Head, Tail...>& tp) {
    in >> tp.head_;
    in >> tp.tail_;
    return in;
  }

  bool operator == (const Tuple<Head, Tail...>& other) {
    if (head_ == other.head_) {
      return tail_ == other.tail_;
    }
    return false;
  }

  std::strong_ordering operator <=> (const Tuple<Head, Tail...>& other) {
    if (head_ < other.head_) {
      return std::strong_ordering::less;
    }
    if (head_ > other.head_) {
      return std::strong_ordering::greater;
    }
    return tail_ <=> other.tail_;
  }

  static constexpr int size = sizeof...(Tail) + 1;

  using ElementType = Head;
  Head head_;
  Tuple<Tail...>& tail_;
};


template<typename T, typename... Args>
T& get(Tuple<Args...>& tp) {
  if constexpr (std::is_same_v<T, typename Tuple<Args...>::ElementType>) {
    return tp.head_;
  } else {
    return get<T>(tp.tail_);
  }
}

template<int index, typename... Args>
auto& get(Tuple<Args...>& tp) {
  if constexpr (index == 0) {
    return tp.head_;
  } else {
    return get<index - 1>(tp.tail_);
  }
}

template<int index, typename... Args>
auto& get(const Tuple<Args...>& tp) {
  if constexpr (index == 0) {
    return tp.head_;
  } else {
    return get<index - 1>(tp.tail_);
  }
}

template <typename... Args>
auto make_tuple(const Args&... args) {
  return Tuple<Args...>(args...);
}

template<int... indexes>
struct Helper {
  template<typename FirstTuple, typename... Args>
  static auto concat_helper(const FirstTuple& first, const Args&... args) {
    if constexpr (FirstTuple::size == sizeof...(indexes)) {
      return Tuple<std::remove_cvref_t<decltype(get<indexes>(std::declval<FirstTuple>()))>..., Args...>(get<indexes>(first)..., args...);
    } else {
      return Helper<indexes..., sizeof...(indexes)>::template concat_helper<
                      FirstTuple, Args...
             >(first, args...);
    }
  }
};


template <typename FirstTuple, typename SecondTuple, int... indexes>
auto tuple_concat(const FirstTuple& first, const SecondTuple& second) {
    if constexpr (SecondTuple::size == sizeof...(indexes)) {
      return Helper<>::concat_helper(first, get<indexes>(second)...);
    } else {
      return tuple_concat<FirstTuple, SecondTuple, indexes..., sizeof...(indexes)>(first, second);
    }
}

template <typename FirstTuple, typename... Tuples>
auto tuple_cat(const FirstTuple& fst, const Tuples&... tpls) {
  if constexpr (sizeof...(Tuples) == 0) {
    return fst;
  } else {
    return tuple_concat(fst, tuple_cat(tpls...));
  }
}

int main() {
  auto x = make_tuple(1,2,3,4,5);

  //std::cin >> x;
  //std::cout << x;
  Tuple<int, double, std::string> tp(1, 2.0, "st");
  std::cout << tuple_cat(x, tp, tp, x);

  std::cout << get<std::string>(tp) << " ";
  std::cout << get<1>(tp);

}
