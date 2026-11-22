#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) std::cin >> values[i];

    // TODO: 取出最大的 k 个数并按降序输出。
    for (int i = 0; i < k; ++i) {
        if (i != 0) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
    return 0;
}
