#include <iostream>
#include <vector> 
#include <string> 
#include <algorithm>
#include <unordered_set>

std::vector<size_t> z_function(std::string s) {
  std::vector<size_t> z(s.length(), 0);
  // aaaabaaa
  // 0 3
  size_t left_z_block = 0;
  size_t right_z_block = 0;
  for (size_t i = 1; i < s.length(); ++i) {
    if (i < right_z_block) {
      z[i] = std::min(right_z_block - i + 1, z[i - left_z_block]);
    }
    while(i + z[i] < s.length() && s[z[i]] == s[i + z[i]]) {
      ++z[i];
    }
    if (i + z[i]  - 1 > right_z_block) {
      left_z_block = i;
      right_z_block = i + z[i]  - 1;
    }
  }
  return z;
}

std::vector<size_t> KnutMorisPratSearcher(const std::string& text, const std::string& pattern) {
  std::vector<size_t> positions;
  std::string helper =  pattern + "#" + text;
  auto z_func = z_function(helper);
  for (size_t i = 0; i < z_func.size(); ++i) {
    if (z_func[i] == pattern.length()) {
      positions.push_back(i - pattern.length() - 1);
    }
  }
  return positions;
}


class ZView {
  public: 
    ZView(const std::string& text, const std::string& pattern) : text(text), pattern(pattern) {
    }

    char operator[](size_t i) const {
      if (i < pattern.length()) {
        return pattern[i];
      }
      if (i > pattern.length()) {
        return text[i - pattern.length() - 1];
      }
      return '#';
    }

    size_t length() {
      return 1 + text.length() + pattern.length();
    }
  private:
    const std::string& text;
    const std::string& pattern;
};


std::vector<size_t> KnutMorisPratSearcherOptimalSpace(const std::string& text, const std::string& pattern) {
   ZView s(text, pattern);
   std::vector<size_t> z(pattern.length(), 0);
  // aaaabaaa
  // 0 3
  size_t left_z_block = 0;
  size_t right_z_block = 0;
  std::vector<size_t> positions;
  
  for (size_t i = 1; i < s.length(); ++i) {
    size_t z_function_value = 0;
    if (i < right_z_block) {
      z_function_value = std::min(right_z_block - i + 1, z[i - left_z_block]);
    }
    while(i + z_function_value < s.length() && s[z_function_value] == s[i + z_function_value]) {
      ++z_function_value;
    }
    if (i + z_function_value  - 1 > right_z_block) {
      left_z_block = i;
      right_z_block = i + z_function_value  - 1;
    }
    if (z_function_value == pattern.length()) {
      positions.push_back(i - pattern.length() - 1);
    }
  }
  return positions;
}

// abababc
// ababab
std::vector<std::string> FindPalindromas(std::string s) {
  std::unordered_set<std::string> palindromas;
  auto rev = s;
  std::reverse(rev.begin(), rev.end());
  auto helper = s + "#" + rev;
  auto z = z_function(helper);
  for (size_t i = s.length() + 1; i < z.size(); ++i) {
    if (z[i] > 0) {
      palindromas.insert(s.substr(0, z[i]));
    }
  }
  return {palindromas.begin(), palindromas.end()};
}

int main() {
  auto v =  KnutMorisPratSearcherOptimalSpace("ahahaaaaahahaaa", "ha");
  for (auto z : v) {
    std::cout << z << " ";
  }
  std::cout << "\n";
  std::string s = "ababacaacababa";
  auto palindras = FindPalindromas(s);
  for (auto ss : palindras) {
    std::cout << ss << "\n";
  }
}
