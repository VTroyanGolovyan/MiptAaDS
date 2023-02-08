#include <iostream>

template <typename T>
class Ptr {
public:
    Ptr(T* y) : x(y){
        //  throw std::runtime_error("rrr"); ЗЛО!!!
    }

    T* operator->() {
        return x;
    }

    T& operator*() {
        return *x;
    }

    T& operator[](size_t i) {
        return *(x + i);
    }

    ~Ptr() {
        delete x;
    }

private:
    T* x;
};

class TestObject {
public:
    TestObject(int val) : x(new int[10]) {
        for (int i = 0; i < 10; ++i) {
            x[i] = val;
        }
    }

    ~TestObject() {
        delete[] x;
        std::cout << "Memory released!";
    }

    int* x;
};

void bad() {
    TestObject* ptr = new TestObject(5);
    throw std::runtime_error("test exception");
    delete ptr; // никогда не выполнится
}

void good() {
    Ptr<TestObject> x(new TestObject(5));
    throw std::runtime_error("test exception");
}
int main() {
    try {
        bad(); // утечка памяти
    } catch (std::runtime_error& err) {
        // ignore
    }
    try {
        good(); // все ок
    } catch (std::runtime_error& err) {
        // ignore
    }
}
