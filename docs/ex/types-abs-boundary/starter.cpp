#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        // TODO: 处理最小负数，并输出绝对值。
        const long long magnitude = value < 0 ? -value : value;
        std::cout << magnitude << '\n';
    }
    return 0;
}
