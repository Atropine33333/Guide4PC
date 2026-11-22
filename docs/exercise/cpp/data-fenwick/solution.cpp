// 参考实现：data-fenwick
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    if (!(std::cin >> n >> q)) return 0;

    std::vector<long long> tree(static_cast<std::size_t>(n) + 1, 0);
    auto add = [&tree, n](int index, long long delta) {
        for (int i = index; i <= n; i += i & -i) tree[static_cast<std::size_t>(i)] += delta;
    };
    auto prefix = [&tree](int index) {
        long long total = 0;
        for (int i = index; i > 0; i -= i & -i) total += tree[static_cast<std::size_t>(i)];
        return total;
    };

    std::string command;
    for (int i = 0; i < q; ++i) {
        std::cin >> command;
        if (command == "add") {
            int index = 0;
            long long delta = 0;
            std::cin >> index >> delta;
            add(index, delta);
        } else if (command == "sum") {
            int index = 0;
            std::cin >> index;
            std::cout << prefix(index) << '\n';
        } else if (command == "range") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            std::cout << prefix(right) - prefix(left - 1) << '\n';
        }
    }
    return 0;
}
