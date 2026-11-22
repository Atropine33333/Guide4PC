// 参考实现：bit-pattern-report
#include <iostream>

int main() {
    unsigned long long x = 0;
    if (!(std::cin >> x)) return 0;

    int setBits = 0;
    unsigned long long highest = 0;

    for (int bit = 0; bit < 32; ++bit) {
        if ((x >> bit) & 1ULL) {
            ++setBits;
            highest = 1ULL << bit;
        }
    }

    const long long signedValue =
        (x >= 2147483648ULL) ? static_cast<long long>(x) - 4294967296LL
                             : static_cast<long long>(x);

    std::cout << setBits << ' ' << highest << ' ' << signedValue << '\n';
    return 0;
}
