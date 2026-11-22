// 参考实现：count-digit-occurrences
#include <iostream>

int main() {
    unsigned long long n = 0;
    int digit = 0;
    if (!(std::cin >> n >> digit)) return 0;

    const unsigned long long d = static_cast<unsigned long long>(digit);
    unsigned long long count = 0;

    for (unsigned long long factor = 1; factor <= n; factor *= 10) {
        const unsigned long long higher = n / (factor * 10);
        const unsigned long long current = (n / factor) % 10;
        const unsigned long long lower = n % factor;

        if (d == 0) {
            if (higher == 0) {
                break;
            }
            if (current > 0) {
                count += higher * factor;
            } else {
                count += (higher - 1) * factor + lower + 1;
            }
        } else {
            if (current > d) {
                count += (higher + 1) * factor;
            } else if (current == d) {
                count += higher * factor + lower + 1;
            } else {
                count += higher * factor;
            }
        }

        if (factor > n / 10) break;
    }

    std::cout << count << '\n';
    return 0;
}
