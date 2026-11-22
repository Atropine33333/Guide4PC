#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<long long> values(static_cast<std::size_t>(n));
    for (long long& value : values) std::cin >> value;

    // TODO: 还没有寻找下一个更大元素。
    for (int i = 0; i < n; ++i) {
        if (i > 0) std::cout << ' ';
        std::cout << -1;
    }
    std::cout << '\n';
    return 0;
}
