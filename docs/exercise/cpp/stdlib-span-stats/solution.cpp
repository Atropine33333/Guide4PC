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
    if (values.empty()) {
        return Stats{0, 0, 0, 0};
    }
    Stats result{values.size(), 0, values.front(), values.front()};
    for (int value : values) {
        result.sum += value;
        if (value < result.min) result.min = value;
        if (value > result.max) result.max = value;
    }
    return result;
}

std::span<const int> trim_zeros(std::span<const int> values) {
    std::size_t begin = 0;
    std::size_t end = values.size();
    while (begin < end && values[begin] == 0) {
        ++begin;
    }
    while (end > begin && values[end - 1] == 0) {
        --end;
    }
    return values.subspan(begin, end - begin);
}
