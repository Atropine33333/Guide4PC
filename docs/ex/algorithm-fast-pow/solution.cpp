// 参考实现：algorithm-fast-pow
#include <iostream>

int main() {
    long long a = 0, b = 0, m = 0;
    std::cin >> a >> b >> m;

    long long result = 1 % m;
    a %= m;
    while (b > 0) {
        if ((b & 1LL) != 0) result = result * a % m;
        a = a * a % m;
        b >>= 1;
    }

    std::cout << result << '\n';
    return 0;
}
