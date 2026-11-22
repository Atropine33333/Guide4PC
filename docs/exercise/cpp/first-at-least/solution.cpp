// 参考实现：first-at-least
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
        const auto it = std::lower_bound(values.begin(), values.end(), x);
        if (it == values.end()) {
            std::cout << n + 1 << '\n';
        } else {
            std::cout << (it - values.begin()) + 1 << '\n';
        }
    }
    return 0;
}
