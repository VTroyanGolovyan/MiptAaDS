#include <iostream>

class EmptyError : public std::runtime_error {
public:
    EmptyError() : std::runtime_error("Empty!") {}
};

int main() {
    throw EmptyError();
    return 0;
}
