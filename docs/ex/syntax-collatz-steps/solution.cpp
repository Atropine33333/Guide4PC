// 参考实现：syntax-collatz-steps
#include <iostream>

int main() {
    long long n = 0;
    std::cin >> n;

    long long steps = 0;
    long long peak = n;
    while (n != 1) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        ++steps;
        if (n > peak) peak = n;
    }

    std::cout << steps << ' ' << peak << '\n';
    return 0;
}
