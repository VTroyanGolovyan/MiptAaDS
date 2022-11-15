#pragma once

#include <list>
#include <unordered_map>
#include <tuple>
#include <functional>
#include <iostream>

namespace std {

  template <size_t from, typename... Args>
  size_t hash_tuple(std::tuple<Args...> tp) {
    if constexpr (sizeof...(Args) == from) {
      return 0;
    } else {
      return std::hash<std::remove_cvref_t<decltype(std::get<from>(tp))>>{}(std::get<from>(tp)) + hash_tuple<from + 1, Args...>(tp);
    }
  }

  template <typename... Args>
  struct hash<std::tuple<Args...>> {
     std::size_t operator()(const std::tuple<Args...>& tp) const noexcept {
        return hash_tuple<0, Args...>(tp);
    }
  };
}

template <size_t Size, typename Callable, typename... Args>
class LRUCache {
  public:
  auto operator()(const Args&... args) {
    std::tuple<Args...> arg_tuple = std::make_tuple(args...);
    if (data_.find(arg_tuple) != data_.end()) {
      return data_.find(arg_tuple)->second;
    }
    if (data_.size() == Size) {
      auto last_used = order_.front();
      order_.pop_front();
      data_.erase(last_used);
    }
    auto result = std::invoke(func, args...);
    data_.insert({arg_tuple, result});
    order_.push_back(arg_tuple);
    return result;
  }

  LRUCache(Callable func_ptr) : func(func_ptr) {}

  ~LRUCache() = default;

  private:
    Callable func;
    std::unordered_multimap<std::tuple<Args...>, decltype(func(std::declval<Args>()...))> data_;
    std::list<std::tuple<Args...>> order_;
};
