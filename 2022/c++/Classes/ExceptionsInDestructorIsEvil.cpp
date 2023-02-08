#include <iostream>
// https://habr.com/ru/post/433944/?ysclid=l8s0b50obt491335773

class ObjectWithEvilExceptionInDestructor {
public:
    ~ObjectWithEvilExceptionInDestructor() {
        throw std::runtime_error("Doctor Evil!");
    }
private:
    int x{2}; // такую оценку вы получите если будете так делать
};

int main() {
    try {
        try {
            ObjectWithEvilExceptionInDestructor x;
            throw std::runtime_error("Haha!");
        } catch (std::runtime_error& err) {
            std::cout << "error\n";
        }
    } catch (std::runtime_error& err) { // поскольку вызывается std terminate - мы не можем ничего отловить даже так
        // ignore
    }

}
