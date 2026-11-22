#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    if (!(std::cin >> n >> q)) return 0;
    std::vector<long long> values(static_cast<std::size_t>(n) + 1, 0);

    std::string command;
    while (std::cin >> command) {
        if (command == "add") {
            int index = 0;
            long long delta = 0;
            std::cin >> index >> delta;
            // TODO: 直接改数组，还没有维护树状数组。
            values[static_cast<std::size_t>(index)] += delta;
        } else if (command == "sum") {
            int index = 0;
            std::cin >> index;
            long long total = 0;
            for (int i = 1; i <= index; ++i) total += values[static_cast<std::size_t>(i)];
            std::cout << total << '\n';
        } else if (command == "range") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            // TODO: 区间查询还没有实现，先固定输出 0。
            std::cout << 0 << '\n';
        }
    }
    return 0;
}
