// 参考实现：algorithm-gcd-lcm
#include <iostream>

namespace {

long long gcd_value(long long a, long long b) {
    while (b != 0) {
        const long long remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

}  // namespace

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        long long a = 0;
        long long b = 0;
        std::cin >> a >> b;

        const long long divisor = gcd_value(a, b);
        const long long multiple = divisor == 0 ? 0 : (a / divisor) * b;
        std::cout << divisor << ' ' << multiple << '\n';
    }
    return 0;
}
