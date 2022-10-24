#include <iostream>
#include <vector>


class AVLTree {
public:
    struct Node {
        Node* left{nullptr};
        Node* right{nullptr};
        int  value;
        unsigned char height{0};
    };

    AVLTree() : root_(nullptr) {}

    void Insert(int value) {
        root_ = Insert(root_, value);
    }

    ~AVLTree() {
        Clear();
        delete root_;
    }

    void Clear() {
        if (root_ != nullptr){
            Clear(root_);
        }
        root_ = nullptr;
    }

    bool Find(int value) {
        return Find(root_, value);
    }

    void Erase(int value) {
        root_ = Erase(root_, value);
    }

private:
    static Node* Erase(Node* node, int value) {
        if (node == nullptr) {
            return node;
        }
        if (node->value == value) {
            if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return FixBalance(temp);
            }
            Node* right_min = FindMin(node->right);
            node->right = UnlinkMin(node->right);
            right_min->left = node->left;
            right_min->right = node->right;
            delete node;
            return FixBalance(right_min);
        }
        if (value < node->value) {
            node->left = Erase(node->left, value);
        } else {
            node->right = Erase(node->right, value);
        }
        return FixBalance(node);
    }

    static Node* FindMin(Node* node) {
        if (node->left == nullptr) {
            return node;
        }
        return FindMin(node->left);
    }

    static Node* UnlinkMin(Node* node) {
        if (node->left == nullptr) {
            return node->right;
        }
        node->left = UnlinkMin(node->left);
        return FixBalance(node);
    }

    static bool Find(Node* node, int value) {
        if (node == nullptr) {
            return false;
        }
        if (node->value == value) {
            return true;
        }
        if (value < node->value) {
            return Find(node->left, value);
        } else {
            return Find(node->right, value);
        }
    }

    static void Clear(Node* node) {
        if (node->left != nullptr) {
            Clear(node->left);
        }
        if (node->right != nullptr) {
            Clear(node->right);
        }
        delete node;
    }

    static Node* Insert(Node* node, int value) {
        if (node == nullptr) {
            Node* new_node = new Node;
            new_node->value = value;
            return new_node;
        }
        if (value <= node->value) {
            node->left = Insert(node->left, value);
        } else {
            node->right = Insert(node->right, value);
        }
        return FixBalance(node);
    }

    static int Height(Node* node) {
        return node == nullptr ? 0 : static_cast<int>(node->height);
    }

    static int BalanceFactor(Node* node) {
        return node == nullptr ? 0 : Height(node->right) - Height(node->left);
    }

    static void CalcHeight(Node* node) {
        if (node == nullptr) {
            return;
        }
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
    }

    static Node* RightRotate(Node* p) {
        Node* q = p->left;
        p->left = q->right;
        q->right = p;
        CalcHeight(p);
        CalcHeight(q);
        return q;
    }

    static Node* LeftRotate(Node* q) {
        Node* p = q->right;
        q->right = p->left;
        p->left = q;
        CalcHeight(q);
        CalcHeight(p);
        return p;
    }

    static Node* FixBalance(Node* node) {
        CalcHeight(node);
        if (BalanceFactor(node) == 2) {
            if (BalanceFactor(node->right) == -1) {
                node->right = RightRotate(node->right);
            }
            return LeftRotate(node);
        }
        if (BalanceFactor(node) == -2) {
            if (BalanceFactor(node->left) == 1) {
                node->left = LeftRotate(node->left);
            }
            return RightRotate(node);
        }
        return node;
    }

    Node* root_{nullptr};
};


int main() {
    AVLTree avl;
    avl.Insert(1);
    avl.Insert(2);
    avl.Insert(3);
    avl.Insert(343);
    for (int i = 5; i < 1000000; ++ i) {
        avl.Insert(i);
    }
    for (int i = 50; i < 100000; ++ i) {
        avl.Erase(i);
    }
    std::cout << avl.Find(3);
    avl.Erase(3);
    std::cout << avl.Find(3);
    int sum = 0;
    for (int i = 50; i < 100000; ++ i) {
        sum += (int)avl.Find(i);
    }
    std::cout << sum;
    return 0;
}
