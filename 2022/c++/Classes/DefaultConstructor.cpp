#include <iostream>

class Foo {
public:
    Foo() {
        std::cout << "DEFAULT";
    }
};

int main() {
    Foo f;
    return 0;
}
