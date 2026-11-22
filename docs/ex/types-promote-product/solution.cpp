// 参考实现：types-promote-product
#include <iostream>

int main() {
    long long a = 0;
    long long b = 0;
    long long c = 0;
    std::cin >> a >> b >> c;

    const long long product = a * b * c;

    std::cout << product << '\n';
    return 0;
}
