// 参考实现：types-unsigned-wrap
#include <iostream>

int main() {
    unsigned long long a = 0;
    unsigned long long b = 0;
    std::cin >> a >> b;

    std::cout << (a - b) << ' ' << (b - a) << '\n';
    return 0;
}
