#include <vector>

int count_distinct(const std::vector<int>& sorted) {
    int count = 0;
    for (std::size_t i = 1; i < sorted.size(); ++i) {
        if (sorted[i] != sorted[i - 1]) ++count;
    }
    // BUG: 空序列以外都少算了第一个值。
    return count;
}
