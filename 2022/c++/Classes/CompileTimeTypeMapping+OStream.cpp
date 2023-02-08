#include <iostream>
#include <cstdio>

template <typename T> // общий случай
struct MapType {
    static inline std::string res{""};
};

template <>
struct MapType<int> { // частичная специализация для инта
    static inline std::string res{"%d"};
};

template <>
struct MapType<float> { // частичная специализация для float
    static inline std::string res{"%f"};
};

class OStreamT {
public:
    template<typename T>
    OStreamT& operator << (const T& x) { //
        printf(MapType<T>::res.c_str(), x); //  мапинг происходит на этапе компиляции
        return *this;
    }

    OStreamT& operator << (const char x[]) { // для вывода литералов строк
        printf("%s", x);
        return *this;
    }
private:

};

static OStreamT mycout;

int main() {
    mycout << "Hello, friend!\n";
    int x = 3;
    mycout << "You mark is " << x << "\n";
    float f = 2.28;
    mycout << "Good float number is " << f << "\n";
    return 0;
}
