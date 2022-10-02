#include <iostream>
#include <vector>
#include <optional>

template <typename T>
class MinHeap {
public:
    MinHeap() : data_(kCapacity), size_(0) {
    }

    std::optional<T> Top() {
        if (size_ == 0) {
            return std::nullopt;
        }
        return data_[0];
    }

    std::optional<T> ExtractMin() {
        if (size_ == 0) {
            return std::nullopt;
        }
        std::swap(data_[0], data_[size_ - 1]);
        --size_;
        SiftDown(0);
        return data_[size_];
    }

    void Insert(T value) {
        if (size_ == data_.size()) {
            data_.resize(2 * size_);
        }
        data_[size_++] = value;
        SiftUp(size_ - 1);
    }

    bool Empty() {
        return size_ == 0;
    }

private:
    void SiftUp(size_t index) {
        if (index > 0) {
            size_t parent = (index - 1) / 2;
            if (data_[index] < data_[parent]) {
                std::swap(data_[index], data_[parent]);
                SiftUp(parent);
            }
        }
    }

    void SiftDown(size_t index) {
        size_t i_min = index;
        size_t i_left_child = index * 2 + 1;
        size_t i_right_child = index * 2 + 2;
        if (i_left_child < size_ && data_[i_left_child] < data_[i_min]) {
            i_min = i_left_child;
        }
        if (i_right_child < size_ && data_[i_right_child] < data_[i_min]) {
            i_min = i_right_child;
        }
        if (index != i_min) {
            std::swap(data_[i_min], data_[index]);
            SiftDown(i_min);
        }
    }

    static constexpr size_t kCapacity = 8;
    std::vector<T> data_;
    size_t size_;
};

template <typename T>
std::vector<T> Merge(const std::vector<std::vector<T>>& arrays) {
    MinHeap<std::pair<T, size_t>> hp;
    std::vector<size_t> indexes(arrays.size(), 0);
    size_t array_index = 0;
    size_t size = 0;
    for (auto& array : arrays) {
        if (!array.empty()) {
            hp.Insert({array[0], array_index});
        }
        size += array.size();
        ++array_index;
    }
    std::vector<T> result;
    result.reserve(size);

    while (!hp.Empty()) {
        auto [value, array_i] = *hp.ExtractMin();
        result.push_back(value);
        ++indexes[array_i];
        if (indexes[array_i] < arrays[array_i].size()) {
            hp.Insert({arrays[array_i][indexes[array_i]], array_i});
        }
    }

    return result;
}

int main() {

    MinHeap<int> hp;

    for (int i = 0; i < 1000000; ++i) {
        hp.Insert(i);
    }
    for (int i = 0; i < 500000; ++i) {
        hp.ExtractMin();
    }
    hp.Insert(-1000);
    std::optional<int> X = hp.ExtractMin();
    if (X.has_value()) {
        std::cout << X.value() << " " << *X << std::endl;
    } else {
        std::cout << "Heap is empty";
    }
    X = hp.Top();
    if (X.has_value()) {
        std::cout << X.value() << " " << *X << std::endl;
    } else {
        std::cout << "Heap is empty";
    }
}
