#include <iostream>

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;

    for (int i = 0; i < m; ++i) {
        int u = 0, v = 0;
        std::cin >> u >> v;
    }

    // TODO: 没有考虑边，直接按编号输出。
    for (int i = 1; i <= n; ++i) std::cout << i << '\n';
    return 0;
}
