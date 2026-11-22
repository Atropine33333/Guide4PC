// 参考实现：semantics-unsigned-compare
#include <iostream>

int main() {
    long long a = 0;
    unsigned long long b = 0;
    std::cin >> a >> b;

    const bool equal = a >= 0 && static_cast<unsigned long long>(a) == b;
    const bool less = a < 0 || static_cast<unsigned long long>(a) < b;

    std::cout << (less ? 1 : 0) << ' ' << (equal ? 1 : 0) << '\n';
    return 0;
}
