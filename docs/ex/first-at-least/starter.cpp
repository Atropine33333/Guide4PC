#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    std::vector<long long> values(n);
    for (long long& value : values) {
        std::cin >> value;
    }

    int q = 0;
    std::cin >> q;

    for (int k = 0; k < q; ++k) {
        long long x = 0;
        std::cin >> x;
        // TODO: 用 std::lower_bound 找到第一个不小于 x 的位置。
        // 如果迭代器等于 values.end()，答案是 n + 1；否则是下标加 1。
        std::cout << 0 << '\n';
    }
    return 0;
}
