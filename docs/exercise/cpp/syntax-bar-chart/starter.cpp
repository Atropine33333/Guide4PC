#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        std::cout << (i + 1) << ": ";
        // TODO: 输出 value 个 '#'；value 为 0 时输出一个 '-'。
        std::cout << '#';
        std::cout << '\n';
    }
    return 0;
}
