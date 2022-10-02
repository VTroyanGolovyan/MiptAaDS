#include <iostream>


/* примерно как могут быть реализованы свои стримы, если вдруг понадобится
 * что-то подобное */
class OStream {
public:
    OStream& operator << (int x) {
        printf("%d", x);
        return *this;
    }

    OStream& operator << (char x[]) {
        printf("%s", x);
        return *this;
    }
};

class Complex {
public:
    Complex() = default;

    Complex(int re, int im) : re_(re), im_(im) {
    }


    friend std::istream&  operator>>(std::istream& in, Complex& x);
    friend OStream&  operator<<(OStream& out, Complex& x);
private:
    int re_{0};
    int im_{0};
};

std::istream&  operator>>(std::istream& in, Complex& x) {
    in >> x.re_ >> x.im_;
    //return in;
    return in;
}

OStream&  operator<<(OStream& out, Complex& x) {
    out << x.re_ << " " << x.im_;
    return out;
}

static OStream MyCout;

int main() {
    Complex x;
    std::cin >> x;
    MyCout << x;
    return 0;
}
