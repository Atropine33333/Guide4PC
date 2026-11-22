#include <iostream>

int main() {
    long long a = 0, b = 0, c = 0;
    std::cin >> a >> b >> c;

    // TODO: 先判断能否组成三角形，再区分三边关系。
    if (a == b && b == c) {
        std::cout << "EQUILATERAL\n";
    } else {
        std::cout << "SCALENE\n";
    }
    return 0;
}
