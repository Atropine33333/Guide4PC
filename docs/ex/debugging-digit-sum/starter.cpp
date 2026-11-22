#include <iostream>

int main() {
    long long n = 0;
    std::cin >> n;

    long long sum = 0;
    while (n > 9) {
        sum += n % 10;
        n /= 10;
    }
    std::cout << sum << '\n';
    return 0;
}
