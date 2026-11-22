#include <string>
#include <variant>

using Value = std::variant<int, double, std::string>;

std::string kind_of(const Value& value) {
    (void)value;
    // TODO: report the active alternative.
    return "unknown";
}

double as_double(const Value& value) {
    (void)value;
    // TODO: convert int, keep double, map string to 0.0.
    return 0.0;
}

std::string describe(const Value& value) {
    (void)value;
    // TODO: build the exact description.
    return "";
}
