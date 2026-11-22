#include <iostream>

int main() {
    unsigned long long a = 0;
    unsigned long long b = 0;
    std::cin >> a >> b;

    // TODO: 直接相减会回绕；不要转成有符号整数输出。
    const long long first = static_cast<long long>(a - b);
    const long long second = static_cast<long long>(b - a);
    std::cout << first << ' ' << second << '\n';
    return 0;
}
