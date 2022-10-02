#include <iostream>

template <typename T, int x> // параметром шаблона могут быть не только типы!
class Test {
public:
    int b{x};
private:
    T a;
};

template<>
class Test<bool, 0> { // частичная специализация

};

class B : public Test<int, 100> { // от частичной специализации можно наследоваться

};

int main() {
    B b;
    std::cout << b.b;
    return 0;
}
