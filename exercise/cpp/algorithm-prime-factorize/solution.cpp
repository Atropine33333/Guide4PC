// 参考实现：algorithm-prime-factorize
#include <iostream>

int main() {
    long long n = 0;
    std::cin >> n;

    bool first = true;
    auto emit = [&first](long long prime, int exponent) {
        if (!first) std::cout << ' ';
        first = false;
        std::cout << prime << '^' << exponent;
    };

    for (long long p = 2; p * p <= n; ++p) {
        if (n % p != 0) continue;
        int exponent = 0;
        while (n % p == 0) {
            n /= p;
            ++exponent;
        }
        emit(p, exponent);
    }
    if (n > 1) emit(n, 1);

    std::cout << '\n';
    return 0;
}
