#include <iostream>
#include <vector>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    std::vector<long long> prefix(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        long long value = 0;
        std::cin >> value;
        // TODO: 建立前缀和，使 prefix[i] = a[1] + … + a[i]。
    }

    int q = 0;
    std::cin >> q;

    for (int k = 0; k < q; ++k) {
        int l = 0;
        int r = 0;
        std::cin >> l >> r;
        // TODO: 用 prefix[r] - prefix[l - 1] 回答并输出。
        std::cout << 0 << '\n';
    }
    return 0;
}
