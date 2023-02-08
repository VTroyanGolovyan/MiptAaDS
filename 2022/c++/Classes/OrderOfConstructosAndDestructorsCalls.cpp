#include <iostream>


class Rectangle {
public:
    Rectangle(int a, int b) : a_(a), b_(b) {
        std::cout << "Parent parametrized ctor\n";
    }

    Rectangle() {
        std::cout << "Parent default ctor\n";
    }

    virtual int area() {
        return a_ * b_;
    }

    virtual ~Rectangle() {
        std::cout << "Parent dtor";
    }

protected:
    int a_;
    int b_;
};

class Square : public Rectangle {
public:
    explicit Square(int a) {
        std::cout << "Child ctor\n";
    }

    int area() override {
        return a_ * b_;
    }

    ~Square() override {
        std::cout << "Child dtor\n";
    }
};

int main() {
    Square q(5);
    return 0;
}
