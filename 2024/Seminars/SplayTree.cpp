#include <iostream>
#include <cassert>
#include <cstdlib>

class SplayTree {
  struct Node {
    Node(int64_t key) : key(key), left(nullptr), right(nullptr), parent(nullptr) {}
    int64_t key;
    Node* left;
    Node* right;
    Node* parent;
  };
public:

  SplayTree() : root_{nullptr} {
  }

  void Insert(int64_t key) {
    InsertImpl(root_, key);
  }

  bool Find(int64_t key) {
    Node* node = FindImpl(root_, key);
    if (node == nullptr) return false;
    Splay(node);
    assert(root_->key == node->key);
    return true;
  }

  void PrintInOrder() {
    PrintInOrderImpl(root_);
  }
  
private:
  /*
  *  Если хотите использовать это в серьез - сделать итеративным
  *  Splay дерево может быть длинным! (может не хватить стека рекурсии)
  */
  void PrintInOrderImpl(Node* node) {
      if (node == nullptr) {
        return;
      }
      PrintInOrderImpl(node->left);
      std::cout << node->key << " ";
      PrintInOrderImpl(node->right);
  }

  Node* FindImpl(Node* node, int64_t key) {
    while (node != nullptr) {
      Node* next = nullptr;
      if (key == node->key) {
        return node;
      }
      if (key <= node->key) {
        next = node->left;
      } else {
        next = node->right;
      }
      node = next;
    }
    return nullptr;
  }

  void InsertImpl(Node* node, int64_t key) {
    if (node == nullptr) {
      root_ = new Node(key);
      return;
    }

    Node* prev = nullptr;
    bool is_left = false;
    while (node != nullptr) {
      Node* next = nullptr;
      if (key <= node->key) {
        next = node->left;
        is_left = true;
      } else {
        next = node->right;
        is_left = false;
      }
      prev = node;
      node = next;
    }
    Node* new_node = new Node(key);
    if (is_left) {
      prev->left = new_node;
      new_node->parent = prev;
    } else {
      prev->right = new_node;
      new_node->parent = prev;
    }
    Splay(new_node);
  }

  void Splay(Node* node) {
    while (Parent(node) != nullptr) {
      // Zig - вызывается только тогда, когда наш родитель корень
      // это возможно только тогда, когда дедушки не существует
      if (GrandParent(node) == nullptr) {
        Zig(node);
      } else if ((IsLeftChild(node) && IsLeftChild(Parent(node))) ||
                  (IsRightChild(node) && IsRightChild(Parent(node)))) {
        ZigZig(node);
      } else if ((IsRightChild(node) && IsLeftChild(Parent(node))) ||
                  (IsLeftChild(node) && IsRightChild(Parent(node)))) {
        ZigZag(node);
      }
    }
    root_ = node;
  }

  Node* Parent(Node* node) {
    if (node == nullptr) {
      return nullptr;
    }
    return node->parent;
  }

  Node* GrandParent(Node* node) {
    return Parent(Parent(node));
  }

  /****************************************
  *                  Zig                  *
  *            Y            X             *
  * rotate -> / \          / \ <- rotate  *
  *          X   C   <=>  A   Y           *
  *         / \              / \          *
  *        A   B            B   C         *
  *****************************************/
  void Zig(Node* node) {
    if (IsLeftChild(node)) {
      RotateRight(Parent(node));
    } else {
      RotateLeft(Parent(node));
    }
  }

  /************************************************************ 
  *                          ZigZig                           *
  *                   1)                    2)                *
  *             Z                  Y                 X        *
  * rotate ->  / \    rotate ->  /   \       =>     / \       *
  *           Y   D     =>      X     Z            A   Y      *
  *          / \               / \   /  \             / \     *
  *         X   C             A   B C    D           B   Z    *
  *        / \                                          / \   *
  *       A   B         <=                   <=        C   D  *
  *************************************************************/
  void ZigZig(Node* node) {
    if (IsLeftChild(node)) {
      RotateRight(GrandParent(node));
      RotateRight(Parent(node));
    } else {
      RotateLeft(GrandParent(node));
      RotateLeft(Parent(node));
    }
  }

  /************************************************************ 
  *                          ZigZag                           *
  *               1)                    2)                    *
  *       Z                    Z                   X          *
  *      / \       rotate ->  / \                /   \        *
  *     Y   D                X   D      =>      Y     Z       *
  *    / \  <- rotate       / \                / \   / \      *
  *   A   X                Y   C              A   B C   D     *
  *      / \      =>      / \                                 *
  *     B   C            A   B                                *
  *************************************************************/
  void ZigZag(Node* node) {
    if (IsRightChild(node)) {
      RotateLeft(Parent(node));
      RotateRight(Parent(node));
    } else {
      RotateRight(Parent(node));
      RotateLeft(Parent(node));
    }
  }

  void RotateRight(Node* p) {
      Node* q = p->left;
      Node* p_par = p->parent;
      // возможна ситуация, когда p - корень
      bool isset = p_par != nullptr;
      // для того, чтобы понять с какой стороны подвешивать q 
      // к родителю p
      bool is_left = isset && p_par->left == p;

      p->left = q->right;
      if (q->right) {
        q->right->parent = p;
      }
      q->right = p;
      p->parent = q;
      q->parent = p_par;
      // переподвешиваем все поддерево к его родителю
      if (isset && is_left) {
        p_par->left = q;
      } else if (isset) {
        p_par->right = q;
      }
    }

    void RotateLeft(Node* q) {
      Node* p = q->right;
      Node* q_par = q->parent;

      bool isset = q_par != nullptr;
      bool is_left = isset && q_par->left == q;

      q->right = p->left;
      if (p->left) {
        p->left->parent = q;
      }
      p->left = q;
      q->parent = p;
      p->parent = q_par;

      // переподвешиваем все поддерево к его родителю
      if (isset && is_left) {
        q_par->left = p;
      } else if (isset) {
        q_par->right = p;
      }
      
    }

  bool IsLeftChild(Node* child) {
    assert(Parent(child) != nullptr);
    return Parent(child)->left == child;
  }

  bool IsRightChild(Node* child) {
    assert(Parent(child) != nullptr);
    return Parent(child)->right == child;
  }

   Node* root_;
};


int main() {
  std::cin.tie(nullptr);
  srand(228);
  int64_t a[10] = {1,-1,2,-2, 100,-100, 200, -200,128,16};
  std::ios_base::sync_with_stdio(false);

  SplayTree bst;

  for (int64_t i = 0; i < 10; i++) {
    bst.Insert(a[i]);
    std::cout << "OK" << std::endl;
    bst.PrintInOrder();
    std::cout << std::endl;
  }

  bst.PrintInOrder();
  return 0;
}
