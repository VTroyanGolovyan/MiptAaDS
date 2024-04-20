#include <iostream>

class BST {
public:
  struct Node {
    explicit Node(int key) : key(key), left(nullptr), right(nullptr) {
    }

    int key;
    int height;

    Node* left;
    Node* right;
  };

  void Insert(int key) {
    root_ = InsertImpl(root_, key);
  }

  bool Contains(int key) {
    return ContainsImpl(root_, key);
  }

  bool Erase(int key) {
    if (!Contains(key)) {
      return false;
    }
    root_ = EraseImpl(root_, key);
    return true;
  }

  ~BST() {
    PostOrderDel(root_);
  }

  void PrintInOrder() {
    PrintInOrderImpl(root_);
  }

  private:

    void PrintInOrderImpl(Node* node) {
      if (node == nullptr) {
        return;
      }
      PrintInOrderImpl(node->left);
      std::cout << node->key << " ";
      PrintInOrderImpl(node->right);
    }

    void PostOrderDel(Node* node) {
      if (node == nullptr) {
        return;
      }
      PostOrderDel(node->left);
      PostOrderDel(node->right);
      delete node;
    }

    bool ContainsImpl(Node* node, int key) {
      if (node == nullptr) {
        return false;
      }
      if (node->key == key) {
        return true;
      }
      if (key <= node->key) {
        return ContainsImpl(node->left, key);
      } else {
        return ContainsImpl(node->right, key);
      }
    }

    Node* InsertImpl(Node* node, int key) {
      if (node == nullptr) {
        return new Node(key);
      }
      if (key <= node->key) {
        node->left = InsertImpl(node->left, key);
      } else {
        node->right = InsertImpl(node->right, key);
      }
      return FixBalance(node);
    }

    Node* EraseImpl(Node* node, int key) {
      if (node == nullptr) {
        return nullptr;
      }
      if (key < node->key) {
        node->left = EraseImpl(node->left, key);
      } else if (key > node->key) {
        node->right = EraseImpl(node->right, key);
      } else {
        
        if (node->right == nullptr || node->left == nullptr) {
          Node* temp = node->left == nullptr ? node->right : node->left;
          delete node;
          return FixBalance(temp);
        }
        
        Node* max_left = FindMax(node->left);
        node->left = UnlinkMax(node->left);

        max_left->left = node->left;
        max_left->right = node->right;
        delete node;
        return FixBalance(max_left);
      }

      return FixBalance(node);
    }

    Node* FindMax(Node* node) {
      if (node->right == nullptr) {
        return node;
      }
      return FindMax(node->right);
    }

    Node* UnlinkMax(Node* node) {
      if (node->right == nullptr) {
        return node->left;
      }
      node->right = UnlinkMax(node->right);
      return FixBalance(node);
    }

    int Height(Node* node) {
      return node == nullptr ? 0 : node->height;
    }

    void FixHeight(Node* node) {
      if (node == nullptr) {
        return;
      }
      node->height = std::max(Height(node->left), Height(node->right)) + 1;
    }

    Node* FixBalance(Node* node) {
      FixHeight(node);
      if (CalcBalance(node) == 2) {
        if (CalcBalance(node->right) == -1) {
          node->right = RotateRight(node->right);
        }
        return RotateLeft(node);
      } 
      if (CalcBalance(node) == -2) {
        if (CalcBalance(node->left) == 1) {
          node->left = RotateLeft(node->left);
        }
        return RotateRight(node);
      }
      return node;
    }

    Node* RotateRight(Node* p) {
      Node* q = p->left;
      p->left = q->right;
      q->right = p;
      FixHeight(p);
      FixHeight(q);
      return q;
    }

    Node* RotateLeft(Node* q) {
      Node* p = q->right;
      q->right = p->left;
      p->left = q;
      FixHeight(q);
      FixHeight(p);
      return p;
    }

    int CalcBalance(Node* node) {
      if (node == nullptr) {
        return 0;
      }
      return Height(node->right) - Height(node->left);
    }

    Node* root_{nullptr};
    size_t size_{0};
};

int main() {
  int n;
  std::cin >> n;
  int x;

  BST bst;

  for (int i = 0; i < n; i++) {
    std::cin >> x;
    bst.Insert(x);
  }

  bst.PrintInOrder();
  return 0;
}
