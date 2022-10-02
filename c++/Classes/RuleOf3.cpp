#include <iostream>


class Vector {
public:
    Vector() : a_(nullptr){}

    Vector(size_t n, int value) : n_(n), a_(new int[n]){
        for (size_t i = 0; i < n; ++i) {
            a_[i] = value;
        }
    }

    //Vector(const Vector& other) = delete;   // запрет копирования.
    //Vector& operator=(const Vector& other) = delete; // запрет копирования.
    Vector(const Vector& other) : a_(new int[other.n_]), n_(other.n_) { // конструктор копирования.
        for (size_t i = 0; i < other.n_; ++i) {
            a_[i] = other.a_[i];
        }
    }

    Vector& operator=(const Vector& other) { // оператор присваивания копированием.
        if (this == std::addressof(other)) { // adressof используется так как оператор & можно перегрузить
            return *this;
        }
        delete[] a_; // нужно очистить старые данные прежде чем копировать новые
        a_ = new int[other.n_];
        for (size_t i = 0; i < other.n_; ++i) {
            a_[i] = other.a_[i];
        }
        n_ = other.n_;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const Vector& vec) {
        for (size_t i = 0; i < vec.n_; ++i) {
            out << vec.a_[i] << " ";
        }
        return out;
    }

    ~Vector() {
        delete []a_;
    }

private:
    int* a_;
    size_t n_;
};


int main() {
    Vector vec1(6, 5);
    Vector vec2 = vec1; // при инициализации вызывается конструктор копирования
    std::cout << vec2 << "\n";
    Vector vec3(7, 7);
    vec2 = vec3; // а тут уже оператор =
    std::cout << vec2 << "\n";
    vec1 = vec1;
    return 0;
}
