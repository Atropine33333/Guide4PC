// 参考实现：count-digit-occurrences
#include <iostream>

int main() {
    long long n = 0;
    int digit = 0;
    if (!(std::cin >> n >> digit)) return 0;

    int count = 0;
    if (n == 0) {
        count = (digit == 0) ? 1 : 0;
    } else {
        while (n > 0) {
            if (n % 10 == digit) ++count;
            n /= 10;
        }
    }
    std::cout << count << '\n';
    return 0;
}
