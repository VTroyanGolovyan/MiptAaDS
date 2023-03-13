#include <iostream>


template <typename KeyT, typename ValueT> 
class AVLTree {
  struct Node {
    int16_t height;
    KeyT key;
    ValueT value;
    Node* left;
    Node* right;
    Node(KeyT key, ValueT value) 
      : key(key), value(value), height(1), left(nullptr), right(nullptr) {
    }
  };

  public:
    AVLTree() : root_(nullptr) {
    }

    void Insert(KeyT key, ValueT value) {
      root_ = Insert(root_, key, value);
    }

    void Erase(KeyT key) {
      root_ = Erase(root_, key);
    }

    bool Contains(KeyT key) {
      return Get(key) != nullptr;
    }

    ValueT* Get(KeyT key) {
      Node* current = root_;
      while (current != nullptr) {
        if (current->key == key) {
          return &current->value;
        }
        current = key < current->key ? current->left : current->right;
      }
      return nullptr;
    }

    void Clear() {
       Clear(root_);
       size_ = 0; 
       root_ = nullptr;
    }

    ~AVLTree() {
      Clear(root_);
    }

    size_t Size() {
      return size_;
    }

  private:

    void Clear(Node* node) {
      if (node == nullptr) {
        return;
      }
      Clear(node->left);
      Clear(node->right);
      delete node;
    }

    Node* Insert(Node* node, KeyT key, ValueT value) {
      if (node == nullptr) {
        ++size_;
        return new Node(key, value);
      }
      if (key == node->key) {
        node->value = value;
        return node;
      } 
      if (key < node->key) {
        node->left = Insert(node->left, key, value);
      } else {
        node->right = Insert(node->right, key, value);
      }
      return Balance(node);
    }

    Node* Erase(Node* node, KeyT key) {
      if (node == nullptr) {
        return node;
      }

      if (key < node->key) {
        node->left = Erase(node->left, key);
        return Balance(node);
      }
      if (node->key < key) {
        node->right = Erase(node->right, key);
        return Balance(node);
      }

      --size_;
      if (node->right == nullptr) {
        Node* left_child = node->left;
        delete node;
        return left_child;
      }

      Node* right_min = FindMin(node->right);
      node->right = DetachMin(node->right);
      std::swap(right_min->key, node->key);
      std::swap(right_min->value, node->value);
      delete right_min;
      return Balance(node);
    }

    Node* Balance(Node* node) {
      ActualizeHeight(node);// при insert/Detach всегда!
      int16_t bf = BalanceFactor(node);
      if (bf == -2) {
        if (BalanceFactor(node->left) == 1) {
          node->left = RotateLeft(node->left);
        }
        return RotateRight(node);
      } else if (bf == 2) {
        if (BalanceFactor(node->right) == -1) {
          node->right = RotateRight(node->right);
        }
        return RotateLeft(node);
      }
      return node;
    }

    Node* RotateLeft(Node* node) {
      Node* new_parent = node->right;
      node->right = new_parent->left;
      new_parent->left = node;
      ActualizeHeight(node);
      ActualizeHeight(new_parent);
      return new_parent;
    }

    Node* RotateRight(Node* node) {
      Node* new_parent = node->left;
      node->left = new_parent->right;
      new_parent->right = node;
      ActualizeHeight(node);
      ActualizeHeight(new_parent);
      return new_parent;
    }

    Node* FindMin(Node* node) {
      Node* curr = node;
      while (curr != nullptr && curr->left != nullptr) {
        curr = curr->left;
      }
      return curr;
    }

    Node* DetachMin(Node* node) {
      if (node == nullptr) {
        return nullptr;
      }
      if (node->left == nullptr) {
        return node->right;
      }
      node->left = DetachMin(node->left);
      return Balance(node);
    }

    int16_t BalanceFactor(Node* node) {
      if (node == nullptr) {
        return 0;
      }
      return (int16_t)Height(node->right) - (int16_t)Height(node->left);
    }

    void ActualizeHeight(Node* node) {
      if (node == nullptr) {
        return;
      }
      node->height = 1 + std::max(Height(node->left), Height(node->right));
    }

    int16_t Height(Node* node) {
      if (node == nullptr) {
        return 0;
      }
      return node->height;
    }

    Node* root_;
    size_t size_{0};
};

int main() {
  AVLTree<int, int> tree;
  for (int i = 0; i < 1000000; ++i) {
    tree.Insert(i, i);
  }
  for (int i = 200000; i < 600000; ++i) {
     tree.Erase(i);
  }
  std::cout << tree.Size();
  return 0;
}
