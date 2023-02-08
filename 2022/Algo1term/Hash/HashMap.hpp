#include <vector>
#include <list>


template <typename T, typename U>
class HashMap {
 public:

  HashMap() : data_(kCapacity, std::list<std::pair<T, U>>()), capacity_(kCapacity), size_(0) {}

  void Insert(const T& key, const U& value) {
    if (Exists(key)) {
      (*this)[key] = value;
      return;
    }
    if (size_ >= 0.75 * capacity_ ) {
      ReserveAndRehash(2 * capacity_);
    }
    ++size_;
    Insert_(data_, capacity_, key, value);
  }

  void Erase(const T& key) {
    if (!Exists(key)) {
      return;
    }

    size_t idx = std::hash<T>{}(key) % capacity_;
    for (auto it = data_[idx].begin(); it != data_[idx].end(); ++it) {
      if (it->first == key) {
        data_[idx].erase(it);
        --size_;
        return;
      }
    }
  }

  bool Exists(const T& key) {
    size_t idx = std::hash<T>{}(key) % capacity_;
    for (auto& item : data_[idx]) {
      if (item.first == key) {
        return true;
      }
    }
    return false;
  }

  U& operator[](const T& key) {
    if (!Exists(key)) { 
      Insert(key, U());
    } 
    size_t idx = std::hash<T>{}(key) % capacity_;
    for (auto& item : data_[idx]) {
      if (item.first == key) {
        return item.second;
      }
    }
    throw std::runtime_error("Error!");
  }

  void ReserveAndRehash(size_t new_capacity) {
    if (new_capacity > capacity_) {
      std::vector<std::list<std::pair<T, U>>> new_data(
        new_capacity, 
        std::list<std::pair<T, U>>()
      );
      for (auto& bucket : data_) {
        for (auto& item : bucket) {
          Insert_(new_data, new_capacity, item.first, item.second);
        }
      }
      data_ = new_data;
      capacity_ = new_capacity;
    }
  }

 private:
   static void Insert_(
    std::vector<std::list<std::pair<T, U>>>& data, 
    size_t capacity,
    const T& key, 
    const U& value
  ) {
    size_t idx = std::hash<T>{}(key) % capacity;
    data[idx].push_back({key, value});
  }

   static constexpr size_t kCapacity{4};
   std::vector<std::list<std::pair<T, U>>> data_;
   size_t size_;
   size_t capacity_;
};