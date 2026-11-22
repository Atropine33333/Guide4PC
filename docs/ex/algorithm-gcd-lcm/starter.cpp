#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        long long a = 0;
        long long b = 0;
        std::cin >> a >> b;
        // TODO: 用欧几里得算法求 gcd，再求 lcm。
        std::cout << a << ' ' << b << '\n';
    }
    return 0;
}
