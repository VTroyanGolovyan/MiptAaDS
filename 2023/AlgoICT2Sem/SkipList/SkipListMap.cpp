#include <random>
#include <iostream>
#include <cassert>


template <typename KeyT, typename ValueT>
class SkipListMap {
  struct ListNode {
    ListNode(KeyT key, ValueT value) 
      : key(key), value(value), right(nullptr), down(nullptr) {}

    KeyT key;
    ValueT value;
    ListNode* right;
    ListNode* down;
  };

  public:
    SkipListMap() : begin_(nullptr), coin_(0.5) {
    }

    void Insert(KeyT key, KeyT value) {
      ListNode* inserted = Insert(begin_, key, value);
      if (inserted != nullptr) {
        ListNode* new_begin = new ListNode(KeyT(), ValueT());
        new_begin->down = begin_;
        begin_ = new_begin;
        begin_->right = new ListNode(key, value);
        begin_->right->down = inserted;
        ++levels_;
      }
    }

    void Erase(KeyT key) {

    }

    ValueT* Get(KeyT key) {
      ListNode* res = Get(begin_, key);
      if (res == nullptr || res->key != key) {
        return nullptr;
      }
      return &res->value;
    }

    void Clear() {
      ListNode* curr_begin = begin_;
      while (curr_begin != nullptr) {
        ListNode* next_lvl = curr_begin->down;
        while (curr_begin != nullptr) {
          ListNode* next_node = curr_begin->right;
          delete curr_begin;
          curr_begin = next_node;
        }
        curr_begin = next_lvl;
      }
    }

    ~SkipListMap() {
      Clear();
    }
  private:

    ListNode* Get(ListNode* node, KeyT key) {
      if (node == nullptr) {
        return nullptr;
      }
      ListNode* curr = node;
      while (curr->right != nullptr && key <= curr->right->key) {
        curr = curr->right;
      }
      if (curr->down == nullptr) {
        return curr;
      } else{
        return Get(curr->down, key);
      }
    }

    ListNode* Insert(ListNode* node, KeyT key, KeyT value) {
      if (node == nullptr) {
        return new ListNode(key, value);
      }
      ListNode* curr = node;
      while (curr->right != nullptr && key <= curr->right->key) {
        curr = curr->right;
      }

      ListNode* down_element = nullptr; 
      if (curr->down != nullptr) {
        down_element = Insert(curr->down, key, value);
      }
      if (curr->down == nullptr || down_element != nullptr) {
        ListNode* new_element = new ListNode(key, value);
        new_element->down = down_element;
        new_element->right = curr->right;
        curr->right = new_element;
        if (NeedGrow()) {
          return new_element;
        }
      }
      return nullptr;
    }

    bool NeedGrow() {
      return coin_(random_);
    }

    size_t levels_;
    ListNode* begin_;
    std::random_device random_;
    std::bernoulli_distribution coin_;
};

int main() {
  SkipListMap<int, int> mp;
  mp.Insert(2,3);
  mp.Insert(34,3);
  mp.Insert(343,353);
  for (int i = 0; i < 1000000; ++i) {
    mp.Insert(500 + i, i);
  }
  mp.Insert(0, 1000);
  std::cout << *mp.Get(0);

  return 0;
}
