// 参考实现：debug-overflow-sum
#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    long long total = 0;
    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        total += value;
    }

    std::cout << total << '\n';
    return 0;
}
