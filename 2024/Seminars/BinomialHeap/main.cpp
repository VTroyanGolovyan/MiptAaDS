#include <algorithm>
#include <iostream>
#include <forward_list>
#include <cassert>
#include <cstdint>
#include <vector>


template <typename T, typename Comparator = std::less<T>>
class BinomialHeap {
    struct BinomialTree {
        explicit BinomialTree(const T& key)
            : key(key), degree(0), parent(nullptr), first_child(nullptr), next_tree(nullptr) {}

        T key;
        unsigned int degree;
        BinomialTree* parent;
        BinomialTree* first_child;
        BinomialTree* next_tree;
    };

public:
    BinomialHeap() = default;

    const T& top() {
        return get_min(heap_trees_)->key;
    }

    bool pop() {
        if (size_ == 0) {
            return false;
        }
        BinomialTree* tree_with_min = get_min(heap_trees_);
        heap_trees_ = remove_node(heap_trees_, tree_with_min);
        BinomialTree* child_heap = reverse_heap(tree_with_min->first_child);
        heap_trees_ = merge_heaps(heap_trees_, child_heap);
        delete tree_with_min;
        --size_;
        return true;
    }

    void push(const T& value) {
        heap_trees_ = merge_heaps(new BinomialTree(value), heap_trees_);
        ++size_;
    }

    ~BinomialHeap() {
        BinomialTree* curr = heap_trees_;
        while (curr != nullptr) {
            BinomialTree* temp = curr;
            curr = curr->next_tree;
            remove_tree(temp);
        }
    }

private:
    void remove_tree(BinomialTree* tree) {
        if (tree == nullptr) {
            return;
        }
        BinomialTree* child = tree->first_child;
        while (child != nullptr) {
            BinomialTree* temp = child;
            child = child->next_tree;
            remove_tree(temp);
        }
        delete tree;
    }

    BinomialTree* remove_node(BinomialTree* head, BinomialTree* to_remove) {
        if (to_remove == heap_trees_) {
            head = heap_trees_->next_tree;
        } else {
            BinomialTree* current = head;
            while (current->next_tree != to_remove) {
                current = current->next_tree;
            }
            current->next_tree = current->next_tree->next_tree;
        }
        return head;
    }

    BinomialTree* get_min(BinomialTree* trees) {
        BinomialTree* curr_tree = heap_trees_;
        BinomialTree* min_tree = heap_trees_;
        while (curr_tree != nullptr) {
            if (cmp_(curr_tree->key, min_tree->key)) {
                min_tree = curr_tree;
            }
            curr_tree = curr_tree->next_tree;
        }
        return min_tree;
    }

    BinomialTree* merge_heaps(BinomialTree* t1, BinomialTree* t2) {
        BinomialTree* merge_result = merge_sorted_by_degree_lists(t1, t2);
        if (merge_result == nullptr) {
            return nullptr;
        }

        BinomialTree* prev = nullptr;
        BinomialTree* current = merge_result;
        BinomialTree* next = current->next_tree;

        while (next != nullptr) {
            if (current->degree != next->degree || (next->next_tree != nullptr && next->next_tree->degree == current->degree)) {
                prev = current;
                current = next;
            } else if (cmp_(current->key, next->key)) {
                current->next_tree = next->next_tree;
                link_binomial(next, current);
            } else {
                if (prev == nullptr) {
                    merge_result = next;
                } else {
                    prev->next_tree = next;
                }
                link_binomial(current, next);
                current = next;
            }
            next = current->next_tree;
        }

        return merge_result;
    }

    void link_binomial(BinomialTree* t1, BinomialTree* t2) {
        assert(t1->degree == t2->degree);
        t1->parent = t2;
        t1->next_tree = t2->first_child;
        t2->first_child = t1;
        ++t2->degree;
    }

    BinomialTree* merge_sorted_by_degree_lists(BinomialTree* t1, BinomialTree* t2) {
        BinomialTree* result = nullptr;
        BinomialTree* current = nullptr;
        while (t1 != nullptr || t2 != nullptr) {
            BinomialTree* new_element = nullptr;
            if (t1 != nullptr && t2 != nullptr && t1->degree < t2->degree) {
                new_element = t1;
                t1 = t1->next_tree;
            } else if (t2 != nullptr) {
                new_element = t2;
                t2 = t2->next_tree;
            } else {
                new_element = t1;
                t1 = t1->next_tree;
            }
            if (result == nullptr) {
                result = new_element;
                current = result;
            } else {
                current->next_tree = new_element;
                current = current->next_tree;
            }
        }
        return result;
    }

    BinomialTree* reverse_heap(BinomialTree* head) {
        if (head == nullptr) {
            return nullptr;
        }
        BinomialTree* prev = nullptr;
        BinomialTree* current = head;
        while (current != nullptr && current->next_tree != nullptr) {
          BinomialTree* next = current->next_tree;
          current->next_tree = prev;
          prev = current;
          current = next;
          current->parent = nullptr;
        }
        current->next_tree = prev;
        current->parent = nullptr;
        return current;

    }

    BinomialTree* heap_trees_{nullptr};
    size_t size_{0};

    Comparator cmp_;
};

int main()
{
    uint64_t n, k;
    std::cin >> n >> k;
    uint64_t a_0, x, y;
    std::cin >> a_0 >> x >> y;

    BinomialHeap<uint64_t, std::greater<>> heap;

    uint64_t a_i = a_0;
    uint64_t mod = 1 << 30;
    for (uint64_t i = 0; i < k; ++i) {
        a_i = (x * a_i + y) % mod;
        heap.push(a_i);
    }
    for (uint64_t i = 0; i < n - k; ++i) {
        a_i = (x * a_i + y) % mod;
        if (a_i < heap.top()) {
            heap.pop();
            heap.push(a_i);
        }
    }
    std::vector<uint64_t> v;
    v.reserve(k);
    for (uint64_t i = 0; i < k; ++i) {
        v.push_back(heap.top());
        heap.pop();
    }
    std::reverse(v.begin(), v.end());
    for (uint64_t i = 0; i < v.size(); ++i) {
        auto x = v[i];
        std::cout << x << " ";
    }

    return 0;
}
