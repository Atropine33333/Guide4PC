// Reference solution: syntax-luhn-check
#include <cstddef>
#include <string>

namespace {

int luhn_sum(const std::string& text, bool double_rightmost) {
    int total = 0;
    bool should_double = double_rightmost;
    for (std::string::const_reverse_iterator it = text.rbegin(); it != text.rend(); ++it) {
        const char ch = *it;
        if (ch < '0' || ch > '9') continue;
        int digit = ch - '0';
        if (should_double) {
            digit *= 2;
            if (digit > 9) digit -= 9;
        }
        total += digit;
        should_double = !should_double;
    }
    return total;
}

bool has_digit(const std::string& text) {
    for (char ch : text) {
        if (ch >= '0' && ch <= '9') return true;
    }
    return false;
}

}  // namespace

bool luhn_valid(const std::string& digits) {
    if (!has_digit(digits)) return false;
    return luhn_sum(digits, false) % 10 == 0;
}

char luhn_check_digit(const std::string& payload) {
    const int total = luhn_sum(payload, true);
    return static_cast<char>('0' + (10 - total % 10) % 10);
}
