#include <iostream>
#include <vector>

int main() {
    int n = 0;
    long long lo = 0, hi = 0;
    std::cin >> n >> lo >> hi;

    std::vector<int> values(static_cast<std::size_t>(n));
    for (int& value : values) std::cin >> value;

    // TODO: 还没有按区间过滤，原样输出。
    std::cout << n << '\n';
    for (int i = 0; i < n; ++i) {
        if (i > 0) std::cout << ' ';
        std::cout << values[static_cast<std::size_t>(i)];
    }
    std::cout << '\n';
    return 0;
}
