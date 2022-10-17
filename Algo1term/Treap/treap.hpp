
#include <iostream>
#include <cstdlib>
#include <utility>

#pragma once

class TreapSet {
  struct Node {
    Node(int x, int y) : x(x), y(y), left(nullptr), right(nullptr) {}

    int x;
    int y;
    Node* left;
    Node* right;

  };

public:
  TreapSet() : root_(nullptr) {}

  ~TreapSet() {
    Clear(root_);
  }

  void Erase(int value) {
    if (!Find(value)) {
      return;
    }
    auto [left_with_value, right] = Split(root_, value);
    auto [left, value_tree] = Split(left_with_value, value - 1);
    delete value_tree;
    root_ = Merge(left, right);
  }

  void EraseFind(int value) {
    auto [parent, value_node] = Find(nullptr, root_, value);
    if (parent == nullptr && value_node != nullptr) {
      root_ = Merge(root_->left, root_->right);
    } else {
      ((parent->left == value_node) ? parent->left : parent->right) = Merge(value_node->left, value_node->right);
    }
    if (value_node != nullptr) {
      delete value_node;
    }
  }

  void Insert(int value) {
    if (Find(value)) {
      return;
    }
    int y = rand();
    Node* node = new Node(value, y);
    auto[first, second] = Split(root_, value);
    root_ = Merge(Merge(first, node), second);
  }

  bool Find(int value) {
    return Find(nullptr, root_, value).second != nullptr;
  }

private:
  std::pair<Node*, Node*> Find(Node* parent, Node* node, int value) {
    if (node == nullptr) {
      return {parent, node};
    }
    if (value == node->x) {
      return {parent, node};
    }
    if (value <= node->x) {
      return Find(node, node->left, value);
    }
    return Find(node, node->right, value);
  }

  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    Clear(node->left);
    Clear(node->right);
    delete node;
  }

  Node* Merge(Node* first, Node* second) {
    if (first == nullptr) {
      return second;
    }
    if (second == nullptr) {
      return first;
    }
    if (first->y > second->y) {
      first->right = Merge(first->right, second);
      return first;
    }
    second->left = Merge(first, second->left);
    return second;
  }

  std::pair<Node*, Node*> Split(Node* node, int key) {
    if (node == nullptr) {
      return {nullptr, nullptr};
    }
    if (key <= node->x) {
      auto[left, right] = Split(node->left, key);
      node->left = right;
      return {left, node};
    }
    auto[left, right] = Split(node->right, key);
    node->right = left;
    return {node, right};
  }

  Node* root_;
};
