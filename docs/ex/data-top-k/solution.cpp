// 参考实现：data-top-k
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    std::priority_queue<int, std::vector<int>, std::greater<int>> smallest;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        smallest.push(value);
        if (static_cast<int>(smallest.size()) > k) smallest.pop();
    }

    std::vector<int> result;
    result.reserve(k);
    while (!smallest.empty()) {
        result.push_back(smallest.top());
        smallest.pop();
    }

    for (int i = k - 1; i >= 0; --i) {
        if (i != k - 1) std::cout << ' ';
        std::cout << result[static_cast<std::size_t>(i)];
    }
    std::cout << '\n';
    return 0;
}
