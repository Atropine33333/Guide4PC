#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    std::vector<int> merged;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        merged.push_back(value);
    }

    int m = 0;
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        int value = 0;
        std::cin >> value;
        // TODO: 把第二个数组也放进 merged。
    }

    // TODO: 排序并去重。

    if (merged.empty()) {
        std::cout << "empty\n";
        return 0;
    }

    for (std::size_t i = 0; i < merged.size(); ++i) {
        if (i != 0) std::cout << ' ';
        std::cout << merged[i];
    }
    std::cout << '\n';
    return 0;
}
