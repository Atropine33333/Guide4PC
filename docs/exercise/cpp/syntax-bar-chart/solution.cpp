// 参考实现：syntax-bar-chart
#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        std::cout << (i + 1) << ": ";
        if (value == 0) {
            std::cout << '-';
        } else {
            for (int j = 0; j < value; ++j) {
                std::cout << '#';
            }
        }
        std::cout << '\n';
    }
    return 0;
}
