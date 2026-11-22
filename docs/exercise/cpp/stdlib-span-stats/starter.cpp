#include <cstddef>
#include <span>
#include <vector>

struct Stats {
    std::size_t count;
    long long sum;
    int min;
    int max;
};

Stats summarize(std::span<const int> values) {
    (void)values;
    return Stats{0, 0, 0, 0};  // TODO
}

std::span<const int> trim_zeros(std::span<const int> values) {
    // TODO: 去掉首尾的零，返回原缓冲区上的子视图。
    return values;
}
