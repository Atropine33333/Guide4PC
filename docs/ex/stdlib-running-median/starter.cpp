#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<long long> seen;
    seen.reserve(n);

    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        seen.push_back(value);

        // TODO: 不要每次整体排序；用两个堆维护下中位数。
        long long median = seen.front();
        std::cout << median;
        if (i + 1 < n) std::cout << ' ';
    }
    std::cout << '\n';
    return 0;
}
