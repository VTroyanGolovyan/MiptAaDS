#include <iostream>

template <typename T, typename U>
T min(const T& a, const U& b) {
    std::cout << "1\n";
    return a > b ? b : a;
}

template <typename T>
T min(const T& a, const T& b) {
    std::cout << "2\n";
    return a > b ? b : a;
}

double min(double x, double y) {
    std::cout << "3\n";
    return x > y ? y : x;
}

int main() {
    min(2, 3.0); // вызывается первая версия функции с найболее общим шаблоном
    min(2,2); // вызывается вторая версия так как типы совпали
    min(2.0, 2.0); // вызывается не шаблонная версия, как найболее подходящая для этого случая
    return 0;
}
