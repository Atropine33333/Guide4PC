#include <iostream>
#include <vector>

int main() {
    int n = 0;
    int k = 0;
    std::cin >> n >> k;
    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) std::cin >> values[i];

    // TODO: 先按 < k 稳定划分，再在右侧按 == k 稳定划分。
    for (int i = 0; i < n; ++i) {
        if (i > 0) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
    return 0;
}
