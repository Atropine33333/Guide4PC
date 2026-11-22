// 参考实现：syntax-diamond-grid
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        const int half = i < n - 1 - i ? i : n - 1 - i;
        const int stars = 2 * half + 1;
        const int spaces = (n - stars) / 2;
        for (int s = 0; s < spaces; ++s) {
            std::cout << ' ';
        }
        for (int s = 0; s < stars; ++s) {
            std::cout << '*';
        }
        std::cout << '\n';
    }
    return 0;
}
