// 参考实现：algorithm-shortest-window
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    long long t = 0;
    std::cin >> n >> t;

    std::vector<long long> values(static_cast<std::size_t>(n));
    for (long long& value : values) std::cin >> value;

    long long window = 0;
    int best = n + 1;
    int left = 0;
    for (int right = 0; right < n; ++right) {
        window += values[static_cast<std::size_t>(right)];
        while (window >= t) {
            best = std::min(best, right - left + 1);
            window -= values[static_cast<std::size_t>(left)];
            ++left;
        }
    }

    std::cout << (best == n + 1 ? 0 : best) << '\n';
    return 0;
}
