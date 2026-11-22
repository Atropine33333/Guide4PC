#include <iostream>

int main() {
    unsigned long long a = 0, b = 0;
    std::cin >> a >> b;

    // TODO: a < b 时直接相减会回绕。
    std::cout << (a - b) << '\n';
    return 0;
}
