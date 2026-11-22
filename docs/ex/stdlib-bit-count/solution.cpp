// 参考实现：stdlib-bit-count
#include <bit>
#include <cstdint>
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        unsigned long long value = 0;
        std::cin >> value;
        const auto bits = static_cast<std::uint32_t>(value);
        std::cout << std::popcount(bits) << '\n';
    }
    return 0;
}
