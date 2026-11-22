// 参考实现：stdlib-running-median
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::priority_queue<long long> lower;
    std::priority_queue<long long, std::vector<long long>, std::greater<long long>> upper;

    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;

        if (lower.empty() || value <= lower.top()) {
            lower.push(value);
        } else {
            upper.push(value);
        }

        if (lower.size() > upper.size() + 1) {
            upper.push(lower.top());
            lower.pop();
        } else if (upper.size() > lower.size()) {
            lower.push(upper.top());
            upper.pop();
        }

        std::cout << lower.top();
        if (i + 1 < n) std::cout << ' ';
    }
    std::cout << '\n';
    return 0;
}
