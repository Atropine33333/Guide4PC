#include <iomanip>
#include <iostream>
#include <string>

int main() {
    int n = 0;
    std::cin >> n;

    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        std::string text;
        std::cin >> text;

        const bool negative = !text.empty() && text[0] == '-';
        long long units = 0;
        for (char c : text) {
            if (c == '.') break;  // TODO: 小数部分被丢掉了。
            if (c >= '0' && c <= '9') units = units * 10 + (c - '0');
        }
        sum += (negative ? -units : units) * 100;
    }

    if (sum < 0) {
        std::cout << '-';
        sum = -sum;
    }
    std::cout << sum / 100 << '.' << std::setw(2) << std::setfill('0')
              << sum % 100 << '\n';
    return 0;
}
