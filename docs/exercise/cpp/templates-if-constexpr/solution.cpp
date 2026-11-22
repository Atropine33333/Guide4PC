// 参考实现：templates-if-constexpr
#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>

template <typename T>
std::string classify(const T& value) {
    (void)value;
    using U = std::decay_t<T>;
    if constexpr (std::is_same_v<U, std::string>) {
        return "string";
    } else if constexpr (std::is_integral_v<U>) {
        return "integral";
    } else if constexpr (std::is_floating_point_v<U>) {
        return "floating";
    } else {
        return "other";
    }
}

template <typename T>
std::string render(const T& value) {
    using U = std::decay_t<T>;
    if constexpr (std::is_same_v<U, std::string>) {
        return value;
    } else if constexpr (std::is_integral_v<U>) {
        return std::to_string(value);
    } else if constexpr (std::is_floating_point_v<U>) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << value;
        return out.str();
    } else {
        (void)value;
        return "?";
    }
}
