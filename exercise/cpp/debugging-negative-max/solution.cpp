// 参考实现：debugging-negative-max
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int first = 0;
    std::cin >> first;
    int best = first;
    for (int i = 1; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        if (value > best) {
            best = value;
        }
    }

    std::cout << best << '\n';
    return 0;
}
