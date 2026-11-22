// 参考实现：stdlib-erase-remove
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    long long lo = 0, hi = 0;
    std::cin >> n >> lo >> hi;

    std::vector<int> kept;
    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        if (lo <= value && value <= hi) {
            kept.push_back(static_cast<int>(value));
        }
    }

    std::cout << kept.size() << '\n';
    if (!kept.empty()) {
        for (std::size_t i = 0; i < kept.size(); ++i) {
            if (i > 0) std::cout << ' ';
            std::cout << kept[i];
        }
        std::cout << '\n';
    }
    return 0;
}
