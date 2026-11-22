// 参考实现：stdlib-optional-parse
#include <optional>
#include <string>

std::optional<int> parse_int(const std::string& text) {
    if (text.empty()) return std::nullopt;

    std::size_t i = 0;
    bool negative = false;
    if (text[0] == '-') {
        negative = true;
        i = 1;
    }
    if (i >= text.size()) return std::nullopt;

    long long value = 0;
    for (; i < text.size(); ++i) {
        const char c = text[i];
        if (c < '0' || c > '9') return std::nullopt;
        value = value * 10 + (c - '0');
        if (value > 2147483648LL) return std::nullopt;
    }

    if (negative) {
        value = -value;
        if (value < -2147483648LL) return std::nullopt;
    } else if (value > 2147483647LL) {
        return std::nullopt;
    }
    return static_cast<int>(value);
}

int parse_or(const std::string& text, int fallback) {
    const std::optional<int> parsed = parse_int(text);
    return parsed.has_value() ? *parsed : fallback;
}
