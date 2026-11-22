// 参考实现：templates-concept-numeric
#include <concepts>
#include <type_traits>
#include <vector>

template <typename T>
concept Numeric = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

template <Numeric T>
T average_of(const std::vector<T>& values, T fallback) {
    if (values.empty()) return fallback;
    T total{};
    for (const T& value : values) total += value;
    return total / static_cast<T>(values.size());
}
