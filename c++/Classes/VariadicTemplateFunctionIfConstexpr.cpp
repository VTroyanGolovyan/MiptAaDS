#include <iostream>

template <typename Head, typename... Tail>
void print(const Head& head, const Tail&... args) {
    std::cout << head << " ";
    if constexpr (sizeof...(args)) { // if contexpr отрабатывает на этапе компиляции!!!!
        print(args...);
    }
}

int main() {
    print(3,4,"Test string", 3.45);
    return 0;
}
