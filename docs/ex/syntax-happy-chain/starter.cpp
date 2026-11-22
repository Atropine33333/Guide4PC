#include <iostream>

int main() {
    long long n = 0;
    std::cin >> n;

    int steps = 0;
    while (n > 1) {
        long long next = 0;
        long long rest = n;
        while (rest > 0) {
            const int digit = static_cast<int>(rest % 10);
            next += digit * digit;
            rest /= 10;
        }
        n = next;
        ++steps;
        if (steps > 1000) {
            // TODO: 检测循环，永远到不了 1 时输出 -1。
            break;
        }
    }

    std::cout << steps << '\n';
    return 0;
}
