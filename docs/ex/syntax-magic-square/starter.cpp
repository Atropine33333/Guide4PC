#include <vector>

std::vector<int> magic_square(int n) {
    // TODO: 为正奇数 n 构造幻方；其他情况返回空数组。
    const int size = n > 0 ? n * n : 0;
    return std::vector<int>(static_cast<std::size_t>(size), 0);
}
