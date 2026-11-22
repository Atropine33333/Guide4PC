#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int best = 0;  // BUG: 全是负数时这个初始值会出错。
    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        if (value > best) {
            best = value;
        }
    }

    std::cout << best << '\n';
    return 0;
}
