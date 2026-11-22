// 参考实现：algorithm-prime-sieve
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    if (n < 2) {
        std::cout << "0 0\n";
        return 0;
    }

    std::vector<char> is_prime(static_cast<std::size_t>(n) + 1, 1);
    is_prime[0] = 0;
    is_prime[1] = 0;
    for (long long p = 2; p * p <= n; ++p) {
        if (!is_prime[static_cast<std::size_t>(p)]) continue;
        for (long long multiple = p * p; multiple <= n; multiple += p) {
            is_prime[static_cast<std::size_t>(multiple)] = 0;
        }
    }

    int count = 0;
    int largest = 0;
    for (int value = 2; value <= n; ++value) {
        if (is_prime[static_cast<std::size_t>(value)]) {
            ++count;
            largest = value;
        }
    }

    std::cout << count << ' ' << largest << '\n';
    return 0;
}
