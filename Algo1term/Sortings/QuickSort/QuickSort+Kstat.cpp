#include <iostream>


template <typename T>
void bubble_sort(T* a, size_t n, size_t dist) {
    size_t t = n;
    bool not_sorted = true;
    for (size_t i = 0; i < n && not_sorted; i += dist) {
        not_sorted = false;
        for (size_t j = dist; j < t; j += dist) {
            if (a[j - dist] > a[j]) {
                std::swap(a[j - dist], a[j]);
                not_sorted = true;
            }
        }
        t -= dist;
    }
}


template <typename T>
std::pair<T, size_t> select_median(T* a, size_t left, size_t right, size_t shift) {
    for (size_t i = left; i < right; i += 5 * shift) {
        bubble_sort(a + i, std::min(right, i + 5 * shift) - i + 1, shift);
    }
    if ((right - left) / shift <= 5) {
        size_t idx = left + std::min((right - left) / shift, 2ul) * shift;
        return {a[idx], idx};
    } else {
        return select_median(a, left, right, shift * 5);
    }
}


template <typename T>
size_t partition(T* a, size_t left, size_t right) {
    //T pivot = a[left];
    //if (right - left > 10) {
    auto [pivot, idx] = select_median(a, left, right, 1ul);
    //}
    std::swap(a[right], a[idx]);


    size_t i = left;
    size_t j = right;
    do {
        while (a[i] < pivot) {
            ++i;
        }
        while (a[j] > pivot) {
            --j;
        }
        if (i >= j) {
            std::swap(a[j], a[right]);
            return j;
        }
        std::swap(a[i], a[j]);
        ++i;
        --j;
    } while (true);
}


template <typename T>
void qsort(T* a, size_t left, size_t right) {
   if (left >= right) {
       return;
   }
   size_t p = partition(a, left, right);
   qsort(a, left, p - 1);
   qsort(a, p + 1, right);
}

template <typename T>
void qsort(T* a, size_t size) {
    qsort(a, 0, size - 1);
}


template <typename T>
T k_stat(T* a, size_t left, size_t right, size_t k) {
    if (left >= right) {
        return a[left];
    }
    size_t p = partition(a, left, right);
    if (p == k) {
        return a[p];
    }
    if (k < p) {
        return k_stat(a, left, p - 1, k);
    } else {
        return k_stat(a, p + 1, right, k);
    }
}


int main() {
    size_t n;
    std::cin >> n;
    int* a = new int[n];
    for (size_t i = 0; i < n; ++i) {
        std::cin >> a[i];
    }


    qsort(a, n);

    std::cout << k_stat(a, 0, n - 1, 4) << std::endl;
    for (size_t i = 0; i < n; ++i) {
        std::cout << a[i] << " ";
    }
    delete [] a;
    return 0;
}
