#include <iostream>


class Grand {
public:
    Grand() {
        std::cout << "Grand\n";
    }
};

class Mom : virtual public Grand {
public:
    Mom() {
        std::cout << "Mom\n";
    }
};

class Dad : virtual public Grand {
public:
    Dad() {
        std::cout << "Dad\n";
    }
};

class Son : public Mom, public Dad {
public:
    Son() {
        std::cout << "Son\n";
    }
};

int main() {
    Son s;
    return 0;
}
