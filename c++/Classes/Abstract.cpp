#include <iostream>

class AbstractClass {
public:
    virtual void draw() = 0; // pure virtual function

    virtual ~AbstractClass() = default;
};

class ResultClass : public AbstractClass {
public:
    void draw() override {
        std::cout << ":)";
    }

    ~ResultClass() override = default;
};

int main() {
    //AbstractClass a; // compilation error
    ResultClass x;
    AbstractClass* ptr = &x; // указатель типа родителя на ребенка
    return 0;
}
