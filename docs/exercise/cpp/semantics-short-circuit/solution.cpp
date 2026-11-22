// 参考实现：semantics-short-circuit
#include <iostream>

int main() {
    int a = 0, b = 0;
    std::cin >> a >> b;

    std::cout << std::boolalpha;
    std::cout << (a < b) << '\n';
    std::cout << (a == b) << '\n';
    std::cout << (a % 2 == 0 && b % 2 == 0) << '\n';
    std::cout << (a != 0 && b / a >= 2) << '\n';
    std::cout << (a == 0 || b % a == 0) << '\n';
    return 0;
}
