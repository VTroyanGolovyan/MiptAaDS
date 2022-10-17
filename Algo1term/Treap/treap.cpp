#include "treap.hpp"


int main () {
  srand(time(nullptr));
  TreapSet tp;
  for (int i = 0; i < 10000000; ++i) {
    tp.Insert(i);
  }
  for (int i = 1000000; i < 2000000; ++i) {
    tp.Erase(i);
  }
  for (int i = 2000000; i < 3000000; ++i) {
    tp.EraseFind(i);
  }
  std::cout << tp.Find(500000);
  std::cout << tp.Find(1500000);
  std::cout << tp.Find(2500000);
  std::cout << std::endl;
  int sum = 0;
  for (int i = 0; i < 10000000; ++i) {
    sum += tp.Find(i);
  }
  std::cout << sum << std::endl;
  for (int i = 10000000; i < 12000000; ++i) {
    tp.Insert(i);
  }
  sum = 0;
  for (int i = 0; i < 12000000; ++i) {
    sum += tp.Find(i);
  }
  std::cout << sum;
}
