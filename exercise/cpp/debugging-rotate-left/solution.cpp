// 参考实现：debugging-rotate-left
#include <algorithm>
#include <cstddef>
#include <vector>

void rotate_left(std::vector<int>& values, int k) {
    const std::size_t n = values.size();
    if (n == 0) return;
    long long shift = static_cast<long long>(k) % static_cast<long long>(n);
    if (shift < 0) shift += static_cast<long long>(n);
    std::rotate(values.begin(), values.begin() + static_cast<std::ptrdiff_t>(shift),
                values.end());
}
