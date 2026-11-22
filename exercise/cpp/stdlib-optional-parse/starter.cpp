#include <optional>
#include <string>

std::optional<int> parse_int(const std::string& text) {
    // TODO: 不处理符号，也不检查范围。
    if (text.empty()) return std::nullopt;
    int value = 0;
    for (char c : text) {
        if (c < '0' || c > '9') return std::nullopt;
        value = value * 10 + (c - '0');
    }
    return value;
}

int parse_or(const std::string& text, int fallback) {
    const std::optional<int> parsed = parse_int(text);
    // TODO: 解析失败时也要返回 fallback。
    return parsed.has_value() ? *parsed : fallback;
}
