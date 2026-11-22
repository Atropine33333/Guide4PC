// 参考实现：types-size-diff
#include <iostream>

int main() {
    unsigned long long a = 0, b = 0;
    std::cin >> a >> b;

    std::cout << (a >= b ? a - b : 0ULL) << '\n';
    return 0;
}
