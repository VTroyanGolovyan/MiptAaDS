#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
class Vector {
public:

    template<bool IsConst>
    class Iterator {
    public:
        using iterator_category = std::contiguous_iterator_tag;
        using iterator_concept=std::contiguous_iterator_tag;
        using self_type=Iterator<IsConst>;
        using difference_type   = ptrdiff_t;
        using value_type        = T;
        using pointer           = std::conditional_t<IsConst, const T*, T*>;
        using reference         = std::conditional_t<IsConst, const T&, T&>;

        Iterator& operator=(const Iterator& other) {
            element_ptr = other.element_ptr;
            return *this;
        };

        Iterator(const Iterator& other) : element_ptr(other.element_ptr) {}
        Iterator& operator=(Iterator&& other) noexcept = default;
        Iterator(Iterator&& other) noexcept = default;
        ~Iterator() = default;

        Iterator& operator++() {
            ++element_ptr;
            return *this;
        }

        Iterator& operator--() {
            --element_ptr;
            return *this;
        }

        Iterator operator++(int) {
            auto copy = *this;
            ++element_ptr;
            return copy;
        }

        Iterator operator--(int) {
            auto copy = *this;
            --element_ptr;
            return copy;
        }

        Iterator& operator+=(ptrdiff_t delta) {
            element_ptr += delta;
            return *this;
        }

        Iterator& operator-=(ptrdiff_t delta) {
            return *this += -delta;
        }

        Iterator operator+(ptrdiff_t delta) const {
            auto result = *this;
            result += delta;
            return result;
        }

        friend Iterator operator+(ptrdiff_t delta, const Iterator& it) {
            return it + delta;
        }

        Iterator operator-(ptrdiff_t delta) const {
            auto result = *this;
            result-=delta;
            return result;
        }

        reference operator*() {
            return *element_ptr;
        }

        pointer operator->() const {
            return element_ptr;
        }

        reference operator[](size_t index) {
            return element_ptr[index];
        }

        friend ptrdiff_t operator-(const Iterator& first, const Iterator& second) {
            return first.element_ptr - second.element_ptr;
        }

        auto operator<=>(const Iterator& other) const {
            if (element_ptr < other.element_ptr) {
                return std::strong_ordering::less;
            }
            if (element_ptr > other.element_ptr) {
                return std::strong_ordering::greater;
            }
            return std::strong_ordering::equal;
        }

        bool operator==(const Iterator&) const = default;
        using element_type      = T;

        friend Vector<T>;
    private:
        Iterator(pointer ptr) : element_ptr(ptr) {
        }

        pointer element_ptr;
    };

    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;
    using value_type = typename iterator::value_type;
    Vector() : data_(nullptr), capacity_(0), size_(0) {
        reserve(8);
    }

    void push_back(const T& value) {
        emplace_back(value);
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        emplace(end(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void emplace_front(Args&&... args) {
        emplace(begin(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void emplace(iterator it, Args&&... args) {
        ptrdiff_t delta = it - begin();
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? kInitialCapacity :  2 * capacity_);
        }
        it = begin() + delta;
        for (auto i = end(); i > it; --i) {
            *i = std::move(*(i - 1));
        }
        new (std::addressof(*it))T(std::forward<Args>(args)...);
        ++size_;
    }

    void reserve(size_t new_capacity) {
        T* new_data = reinterpret_cast<T*>(new uint8_t[sizeof(T) * new_capacity]);
        for (size_t i = 0; i < size_; ++i) {
            new (new_data + i)T(std::move(data_[i]));
        }
        delete[] data_;
        capacity_ = new_capacity;
        data_ = new_data;
    }

    iterator begin() {
        return {data_};
    }

    iterator end() {
        return {data_ + size_};
    }

    const_iterator cbegin() {
        return {data_};
    }

    const_iterator cend() {
        return {data_ + size_};
    }

    auto rbegin() {
        return std::reverse_iterator<iterator>(end());
    }

    auto rend() {
        return std::reverse_iterator<iterator>(begin());
    }

    auto crbegin() {
        return std::reverse_iterator<const_iterator>(cend());
    }

    auto crend() {
        return std::reverse_iterator<const_iterator>(cbegin());
    }

private:
    static constexpr size_t kInitialCapacity = 8;
    T* data_;
    size_t capacity_;
    size_t size_;
};

int main() {
    srand(time(nullptr));

    Vector<size_t> v;
    auto t = v.begin();
   for (size_t i = 0; i < 10; ++i) {
        v.emplace_back(rand() % 100);
   }

   //std::cout << std::count(v.begin(), v.end(), 3) << "\n";
   std::sort(v.begin(), v.end());
   for (auto&& el : v) {
       std::cout << el << " ";
   }


    return 0;
}
