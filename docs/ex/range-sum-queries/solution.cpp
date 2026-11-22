// 参考实现：range-sum-queries
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    std::vector<long long> prefix(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        long long value = 0;
        std::cin >> value;
        prefix[i] = prefix[i - 1] + value;
    }

    int q = 0;
    std::cin >> q;

    for (int k = 0; k < q; ++k) {
        int l = 0;
        int r = 0;
        std::cin >> l >> r;
        std::cout << prefix[r] - prefix[l - 1] << '\n';
    }
    return 0;
}
