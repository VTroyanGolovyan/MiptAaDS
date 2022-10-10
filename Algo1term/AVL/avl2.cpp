#include <iostream>


class AVLTree {
    struct Node {
        Node* left{nullptr};
        Node* right{nullptr};
        int value;
        unsigned char height{0};
    };

public:
    void Insert(int value) {
        root_ = Insert(root_, value);
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
            return nullptr;
        }
        if (node->value == value) {
            if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return FixBalance(temp);
            }
            Node* min_right = FindMin(node->right);
            node->right = UnlinkMin(node->right);
            min_right->left = node->left;
            min_right->right = node->right;
            delete node;
            return FixBalance(min_right);
        }
        if (value <= node->value) {
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
        if (value <= node->value) {
            return Find(node->left, value);
        } else {
            return Find(node->right, value);
        }
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


    static void FixHeight(Node* node) {
        if (node == nullptr) {
            return;
        }
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
    }

    static Node* RotateRight(Node* p) {
        Node* q = p->left;
        p->left = q->right;
        q->right = p;
        FixHeight(p);
        FixHeight(q);
        return q;
    }

    static Node* RotateLeft(Node* q) {
        Node* p = q->right;
        q->right = p->left;
        p->left = q;
        FixHeight(q);
        FixHeight(p);
        return p;
    }

    static Node* FixBalance(Node* node) {
        FixHeight(node);
        if (BalanceFactor(node) == 2) {
            if (BalanceFactor(node->right) == -1) {
                node->right = RotateRight(node->right);
            }
            return RotateLeft(node);
        }
        if (BalanceFactor(node) == -2) {
            if (BalanceFactor(node->left) == 1) {
                node->left = RotateLeft(node->left);
            }
            return RotateRight(node);
        }
        return node;
    }

    static int Height(Node* node) {
        return node == nullptr ? 0 : node->height;
    }

    static int BalanceFactor(Node* node) {
        return node == nullptr ? 0 : Height(node->right) - Height(node->left);
    }

    Node* root_{nullptr};
};
int main() {
    AVLTree a;
    for (int i = 0; i < 10000000; ++i) {
        a.Insert(i);
    }

    for (int i = 500000; i < 4000000; ++i) {
        a.Erase(i);
    }

    int sum = 0;
    for (int i = 500000; i < 800000; ++i) {
        sum += a.Find(i);
    }
    std::cout << sum;

    return 0;
}
