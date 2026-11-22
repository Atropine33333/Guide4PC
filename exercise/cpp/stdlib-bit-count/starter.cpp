#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        unsigned long long value = 0;
        std::cin >> value;
        // TODO: 统计 32 位中 1 的个数。
        const int count = static_cast<int>(value & 1ULL);
        std::cout << count << '\n';
    }
    return 0;
}
