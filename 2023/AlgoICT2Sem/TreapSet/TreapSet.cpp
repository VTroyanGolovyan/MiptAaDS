#include <random>
#include <iostream>
#include <cassert>

template <typename TKey>
class TreapSet {

  struct TreapNode {
    TreapNode(TKey key, int priority) 
      : key(key), priority(priority), left(nullptr), right(nullptr) {
    }

    TKey key; 
    int priority;   
    TreapNode* left;
    TreapNode* right;
  };

  struct TreePair {
    TreapNode* t1;
    TreapNode* t2;
  };

  public:
    TreapSet() : root_(nullptr), size_(0) {
    }

    ~TreapSet() {
      Clear(root_);
    }

    void Insert(TKey key) {
      if (Contains(key)) {
        return;
      }
      ++size_;
      auto [t1, t2] = Split(root_, key);
      TreapNode* new_node = new TreapNode(
        key, random_dist_(random_));
      root_ = Merge(Merge(t1, new_node), t2);
    }
 
    void Erase(TKey key) {
      root_ = Erase(root_, key);
    }

    bool Contains(TKey key) {
      return Find(root_, key) != nullptr;
    }

    void Clear() {
      Clear(root_);
      root_ = nullptr;
      size_ = 0;
    }

    size_t Size() {
      return size_;
    }
  private:

    void Clear(TreapNode* tree) {
      if (tree == nullptr) {
        return;
      }
      Clear(tree->left);
      Clear(tree->right);
      delete tree;
    }

    TreapNode* Erase(TreapNode* tree, TKey key) {
      if (tree == nullptr) {
        return nullptr;
      }
      if (tree->key == key) {
        TreapNode* new_tree = Merge(tree->left, tree->right);
        delete tree;
        --size_;
        return new_tree;
      }
      if (key < tree->key) {
        tree->left = Erase(tree->left, key);
      } else {
        tree->right = Erase(tree->right, key);
      }
      return tree;
    }

    TreapNode* Find(TreapNode* tree, TKey key) {
      if (tree == nullptr) {
        return tree;
      }
      if (tree->key == key) {
        return tree;
      }
      if (key < tree->key) {
        return Find(tree->left, key);
      } else {
        return Find(tree->right, key);
      }
    }

    TreePair Split(TreapNode* tree, TKey key) {
      if (tree == nullptr) {
        return {nullptr, nullptr};
      }
      if (key < tree->key) {
        auto [first, second] = Split(tree->left, key);
        tree->left = second;
        return {first, tree};
      } else {
        auto [first, second] = Split(tree->right, key);
        tree->right = first;
        return {tree, second};
      }
    }

    /* All keys in t1 < t2 */
    TreapNode* Merge(TreapNode* t1, TreapNode* t2) {
      if (t1 == nullptr) {
        return t2;
      }
      if (t2 == nullptr) {
        return t1;
      }
      if (t1->priority < t2->priority) {
        t2->left = Merge(t1, t2->left);
        return t2;
      } else {
        t1->right = Merge(t1->right, t2);
        return t1;
      }
    }

    TreapNode* root_;
    size_t size_;
    std::random_device random_;
    std::uniform_int_distribution<int> random_dist_;
};

int main() {
  TreapSet<int> set;
  for (int i = 0; i < 100000; ++i) {
    set.Insert(i);
  }
  for (int i = 0; i < 100000; ++i) {
    set.Insert(i);
  }
  for (int i = 0; i < 50000; ++i) {
    set.Erase(i);
  }
  
  int cnt = 0;
  for (int i = 0; i < 50000; ++i) {
    if (set.Contains(i)) {
      ++cnt;
    }
  }
  assert(cnt == 0);

  cnt = 0;
  for (int i = 50000; i < 100000; ++i) {
    if (set.Contains(i)) {
      ++cnt;
    }
  }
  assert(cnt == 50000);

  for (int i = 50000; i < 100000; i += 2) {
     set.Erase(i);
  }

  cnt = 0;

  for (int i = 50000; i < 100000; i += 1) {
     if (set.Contains(i)) {
      ++cnt;
    }
  }
  assert(cnt == 25000);
  assert(set.Size() == 25000);
  set.Clear();
  for (int i = 0; i < 1000; ++i) {
    set.Insert(i);
  }
  assert(set.Size() == 1000);
}
