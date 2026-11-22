#include <cstddef>

template <typename T, int N>
double average(const T (&values)[N]) {
    (void)values;
    // TODO: 累加到 double，再除以 N。
    return 0.0;
}

template <typename T, int N>
T max_value(const T (&values)[N]) {
    (void)values;
    // TODO: 返回最大元素。
    return values[0];
}
