// 参考实现：semantics-range-modify
#include <vector>

void scale_in_place(std::vector<int>& values, int factor) {
    for (int& value : values) {
        value *= factor;
    }
}

long long sum_positive(const std::vector<int>& values) {
    long long total = 0;
    for (int value : values) {
        if (value > 0) total += value;
    }
    return total;
}

int count_multiples(const std::vector<int>& values, int divisor) {
    int count = 0;
    for (int value : values) {
        if (value % divisor == 0) ++count;
    }
    return count;
}
