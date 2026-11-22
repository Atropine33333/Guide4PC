#include <iostream>

int main() {
    long long a = 0;
    unsigned long long b = 0;
    std::cin >> a >> b;

    // TODO: 先处理 a < 0 的情况，再在无符号域里比较。
    const bool less = a < b;
    const bool equal = a == b;

    std::cout << (less ? 1 : 0) << ' ' << (equal ? 1 : 0) << '\n';
    return 0;
}
