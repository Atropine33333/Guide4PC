#include <concepts>
#include <type_traits>
#include <vector>

template <typename T>
concept Numeric = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

template <Numeric T>
T average_of(const std::vector<T>& values, T fallback) {
    (void)values;
    // TODO: 还没有计算平均值。
    return fallback;
}
