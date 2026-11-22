// Reference solution: stdlib-variant-visit
#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

using Value = std::variant<int, double, std::string>;

std::string kind_of(const Value& value) {
    return std::visit(
        [](const auto& item) -> std::string {
            using T = std::decay_t<decltype(item)>;
            if constexpr (std::is_same_v<T, int>) {
                return "int";
            } else if constexpr (std::is_same_v<T, double>) {
                return "double";
            } else {
                return "string";
            }
        },
        value);
}

double as_double(const Value& value) {
    return std::visit(
        [](const auto& item) -> double {
            using T = std::decay_t<decltype(item)>;
            if constexpr (std::is_same_v<T, int>) {
                return static_cast<double>(item);
            } else if constexpr (std::is_same_v<T, double>) {
                return item;
            } else {
                return 0.0;
            }
        },
        value);
}

std::string describe(const Value& value) {
    return std::visit(
        [](const auto& item) -> std::string {
            using T = std::decay_t<decltype(item)>;
            if constexpr (std::is_same_v<T, int>) {
                return "int(" + std::to_string(item) + ")";
            } else if constexpr (std::is_same_v<T, double>) {
                std::ostringstream out;
                out << std::fixed << std::setprecision(2) << item;
                return "double(" + out.str() + ")";
            } else {
                return "string(" + item + ")";
            }
        },
        value);
}
