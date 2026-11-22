#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    long long best = 0;
    long long current = 0;
    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        // TODO: 初始值不能是 0，否则全负数会得到错误答案。
        current = (current + value > value) ? current + value : value;
        if (current > best) best = current;
    }

    std::cout << best << '\n';
    return 0;
}
