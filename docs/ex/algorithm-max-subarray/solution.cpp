// 参考实现：algorithm-max-subarray
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    long long first = 0;
    std::cin >> first;
    long long best = first;
    long long current = first;

    for (int i = 1; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        current = (current + value > value) ? current + value : value;
        if (current > best) best = current;
    }

    std::cout << best << '\n';
    return 0;
}
