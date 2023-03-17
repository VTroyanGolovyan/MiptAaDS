#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>

static constexpr size_t kAlphabetSize = 256;


std::vector<int64_t> SuffixArray(
  const std::string& str
) {
  std::string helper = str + '\0'; // чтобы сортить циклические перестановки
  size_t n = helper.length();

  std::array<int64_t, kAlphabetSize> cnt = {}; // для сортировки подсчетом
  for (auto symbol : helper) {
    ++cnt[symbol];
  }
  for (int64_t i = 1; i < cnt.size(); ++i) {
    cnt[i] += cnt[i - 1];
  }

  std::vector<int64_t> transposition(n); // перестановка циклических сдвигов
  
  for (int64_t i = n - 1; i >= 0; --i) {
     transposition[--cnt[helper[i]]] = i;   // устойчивая сортировка подсчетом
  }
  std::vector<int64_t> classes(n);       // классы эквивалентности 
  int64_t current_class = 0;
  for (int64_t i = 0; i < n; ++i) {
    if (i > 0 && helper[transposition[i]] != helper[transposition[i - 1]]) {
      ++current_class;
    }
    classes[transposition[i]] = current_class;  // transposition[i] - текущий класс
  }
   
  size_t k = 1;
  while ((1 << k) <= n) {  // имеет смысл сортить сдвиги длинны до 2^k
    std::vector<int64_t> cnt(n, 0);  
    std::vector<int64_t> new_transposition(n, 0); // перестановка строк длины 2^(k)
    // transposition - перестановка строк длины 2^(k - 1)

    // скока каких классов в строке
    for (int64_t i = 0; i < n; ++i) {
      ++cnt[classes[transposition[i]]];
    }
    for (int64_t i = 1; i < n; ++i) {
      cnt[i] += cnt[i - 1];
    }

    // нужно сформировать новую перестановку
    for (int64_t i = n - 1; i >= 0; --i) {
      // индекс предыдущего элемента в сортировке
      int64_t j = transposition[i] - (1 << (k - 1));
      if (j < 0) {
        j += n;
      }
      new_transposition[--cnt[classes[j]]] = j; // ставим начало сдвига куда нужно
    }

    size_t now_class = 0;

    std::vector<int64_t> new_classes(n, 0); // новые классы эквивалентности
    new_classes[new_transposition[0]] = 0;
    for (int64_t i = 1; i < n; ++i) {
      if (classes[new_transposition[i]] != classes[new_transposition[i - 1]] ||
         classes[(new_transposition[i] + (1 << (k - 1))) % n] 
         != classes[(new_transposition[i - 1] + (1 << (k - 1))) % n]
      ) {
        ++now_class;
      }
      new_classes[new_transposition[i]] = now_class;
    }

    transposition = std::move(new_transposition);
    classes = std::move(new_classes);
    ++k;
  }

  return transposition;    
}

int main() {
  
  std::string tst;
  std::getline(std::cin, tst);
  auto result = SuffixArray(tst);
  for (size_t i = 1; i < result.size(); ++i) {
    std::cout << result[i]  + 1<< " ";
    
  }

  // построим суффмасс для сверки с помощью std sort
  std::cout << "\nWith sort:\n"; 
  std::vector<std::string> a;
  for (size_t i = 0; i < tst.length(); ++i) {
    a.push_back(tst.substr(i));
  }
  std::cout << "stdsort\n";
  std::sort(a.begin(), a.end());
  for (auto x : a) {
    std::cout << (tst.length() - x.length() + 1) << " ";
  }


  return 0;
}

