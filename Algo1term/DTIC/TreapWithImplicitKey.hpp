
#include <iostream>
#include <cstdlib>
#include <utility>
#include <random>

#pragma once

template <typename T>
class TreapArray {
  struct Node {
    Node(int64_t priority, const T& value) 
      : size(1), 
        priority(priority),
        value(value),
        left(nullptr), 
        right(nullptr) {}

    int64_t size;
    int64_t priority;
    T value;
    Node* left;
    Node* right;
  };

public:
  TreapArray() : root_(nullptr) {}

  ~TreapArray() {
    Clear(root_);
  }

  int64_t Size() {
    return Size(root_);
  }

  bool Empty() {
    return Size(root_) == 0;
  }

  void Erase(int64_t pos) {
    auto [left, right_with_pos] = Split(root_, pos);
    auto [pos_tree, right] = Split(right_with_pos, 1);
    delete pos_tree;
    root_ = Merge(left, right);
  }

  void Insert(int64_t pos, const T& value) {
    int64_t priority = distribution_(gen_);
    Node* node = new Node(priority, value);
    auto[first, second] = Split(root_, pos);
    root_ = Merge(Merge(first, node), second);
  }

  T& operator[](int64_t pos) {
    auto [parent, pos_node] = Find(nullptr, root_, pos);
    return pos_node->value;
  }
private:
  std::pair<Node*, Node*> Find(Node* parent, Node* node, int64_t pos) {
    if (node == nullptr) {
      return {parent, node};
    }
    int64_t left_size = Size(node->left);
    if (pos == left_size) {
      return {parent, node};
    }
    if (pos < left_size) {
      return Find(node, node->left, pos);
    }
    return Find(node, node->right, pos - left_size - 1);
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
    if (first->priority > second->priority) {
      first->right = Merge(first->right, second);
      Update(first);
      return first;
    }
    second->left = Merge(first, second->left);
    Update(second);
    return second;
  }

  std::pair<Node*, Node*> Split(Node* node, int64_t pos) {
    if (node == nullptr) {
      return {nullptr, nullptr};
    }
    int64_t left_size = Size(node->left);
    if (pos <= left_size) {
      auto[left, right] = Split(node->left, pos);
      node->left = right;
      Update(node);
      return {left, node};
    }
    auto[left, right] = Split(node->right, pos - left_size - 1);
    node->right = left;
    Update(node);
    return {node, right};
  }

  void Update(Node* node) {
    if (node == nullptr) {
      return;
    }
    node->size = 1 + Size(node->left) + Size(node->right);
  }

  int64_t Size(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->size;
  }

  Node* root_;
  std::mt19937 gen_;
  std::uniform_int_distribution<int64_t> distribution_;
};