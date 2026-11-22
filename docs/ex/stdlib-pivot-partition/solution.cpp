// 参考实现：stdlib-pivot-partition
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    int k = 0;
    if (!(std::cin >> n >> k)) return 0;
    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) std::cin >> values[i];

    auto middle = std::stable_partition(values.begin(), values.end(),
                                        [k](int value) { return value < k; });
    std::stable_partition(middle, values.end(), [k](int value) { return value == k; });

    for (int i = 0; i < n; ++i) {
        if (i > 0) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
    return 0;
}
