#include "crng.hpp"
#include <iostream>

int main() {
    const auto id = crng::GenerateId22Base63();
    std::string str;
    str.append(id.data(), std::next(id.data(), id.size()));
    str[0] = 'u';
    std::cerr << str.size() << "/" << str.capacity() << ":";
    std::cout << str << "\n";
    return 0;
}
