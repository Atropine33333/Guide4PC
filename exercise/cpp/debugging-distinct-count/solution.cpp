// 参考实现：debugging-distinct-count
#include <vector>

int count_distinct(const std::vector<int>& sorted) {
    if (sorted.empty()) return 0;
    int count = 1;
    for (std::size_t i = 1; i < sorted.size(); ++i) {
        if (sorted[i] != sorted[i - 1]) ++count;
    }
    return count;
}
