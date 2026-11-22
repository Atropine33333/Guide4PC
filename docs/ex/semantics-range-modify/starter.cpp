#include <vector>

void scale_in_place(std::vector<int>& values, int factor) {
    for (int value : values) {
        (void)value;
        (void)factor;
        // TODO: 这里拿到的是副本。
    }
}

long long sum_positive(const std::vector<int>& values) {
    (void)values;
    // TODO: 统计所有正数之和。
    return 0;
}

int count_multiples(const std::vector<int>& values, int divisor) {
    (void)values;
    (void)divisor;
    // TODO: 统计能被 divisor 整除的元素。
    return 0;
}
