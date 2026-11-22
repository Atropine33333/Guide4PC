// 参考实现：types-fixed-point
#include <iomanip>
#include <iostream>
#include <string>

namespace {

long long parse_cents(const std::string& text) {
    std::size_t i = 0;
    bool negative = false;
    if (i < text.size() && text[i] == '-') {
        negative = true;
        ++i;
    }

    long long units = 0;
    while (i < text.size() && text[i] != '.') {
        units = units * 10 + (text[i] - '0');
        ++i;
    }

    long long cents = 0;
    if (i < text.size() && text[i] == '.') {
        ++i;
        if (i < text.size()) {
            cents += (text[i] - '0') * 10;
            ++i;
        }
        if (i < text.size()) {
            cents += (text[i] - '0');
            ++i;
        }
    }

    const long long total = units * 100 + cents;
    return negative ? -total : total;
}

}  // namespace

int main() {
    int n = 0;
    std::cin >> n;

    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        std::string text;
        std::cin >> text;
        sum += parse_cents(text);
    }

    if (sum < 0) {
        std::cout << '-';
        sum = -sum;
    }
    std::cout << sum / 100 << '.' << std::setw(2) << std::setfill('0')
              << sum % 100 << '\n';
    return 0;
}
