// 参考实现：syntax-base-convert
#include <string>
#include <algorithm>

std::string to_base(long long value, int base) {
    if (base < 2 || base > 36) return "";
    if (value == 0) return "0";

    const bool negative = value < 0;
    unsigned long long magnitude = negative
        ? 0ULL - static_cast<unsigned long long>(value)
        : static_cast<unsigned long long>(value);

    std::string digits;
    while (magnitude > 0) {
        const unsigned digit = static_cast<unsigned>(magnitude % static_cast<unsigned>(base));
        digits.push_back(digit < 10 ? static_cast<char>('0' + digit)
                                    : static_cast<char>('A' + (digit - 10)));
        magnitude /= static_cast<unsigned>(base);
    }
    if (negative) digits.push_back('-');
    std::reverse(digits.begin(), digits.end());
    return digits;
}

bool parse_base(const std::string& text, int base, long long& out) {
    if (base < 2 || base > 36 || text.empty()) return false;

    std::size_t index = 0;
    bool negative = false;
    if (text[0] == '+' || text[0] == '-') {
        negative = text[0] == '-';
        index = 1;
    }
    if (index == text.size()) return false;

    const unsigned long long limit = negative ? (1ULL << 63) : (1ULL << 63) - 1;
    unsigned long long total = 0;
    for (; index < text.size(); ++index) {
        const char c = text[index];
        int digit = -1;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z') {
            digit = c - 'a' + 10;
        }
        if (digit < 0 || digit >= base) return false;
        const unsigned long long d = static_cast<unsigned long long>(digit);
        if (total > (limit - d) / static_cast<unsigned long long>(base)) return false;
        total = total * static_cast<unsigned long long>(base) + d;
    }

    if (negative) {
        out = static_cast<long long>(0ULL - total);
    } else {
        out = static_cast<long long>(total);
    }
    return true;
}
