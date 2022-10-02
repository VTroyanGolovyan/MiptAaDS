#include <iostream>

class Vector {
public:
    Vector(int n, int value) : a(new int[n]) {
        for (int i = 0; i < n; ++i) {
            a[i] = value;
        }
    }

    Vector(Vector&& other) noexcept : a(other.a), n(other.n) { // конструктор перемещения
        other.a = nullptr;
        other.n = 0;
    }

    Vector& operator=(Vector&& other) noexcept { // оператор присваивания перемещением
        if (std::addressof(other) == this) {
            return *this;
        }
        delete[] a;
        a = other.a;
        n = other.n;
        other.a = nullptr;
        other.n = 0;
        return *this;
    }

    Vector(const Vector& other) : n(other.n), a(new int[other.n]) {
        for (int i = 0; i < other.n; ++i) {
            a[i] = other.a[i];
        }
    }

    Vector& operator=(const Vector& other) {
        if (std::addressof(other) == this) {
            return *this;
        }
        delete[] a;
        a = new int[other.n];
        for (int i = 0; i < other.n; ++i) {
            a[i] = other.a[i];
        }
        n = other.n;
        return *this;
    }

    int& operator[] (size_t index) {
        return a[index];
    }

    ~Vector() {
        delete[] a;
    }

private:
    int n;
    int* a;
};

int main() {
    int&& a = 5; // rvalue ссылка
    Vector b(100, 14);
    auto c = std::move(b); // перемещение
    std::cout << c[2];
    return 0;
}
