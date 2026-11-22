// 参考实现：io-thousands-separator
#include <iostream>
#include <string>

int main() {
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;

        const bool negative = value < 0;
        const unsigned long long magnitude =
            negative ? static_cast<unsigned long long>(-(value + 1)) + 1ULL
                     : static_cast<unsigned long long>(value);
        std::string digits = std::to_string(magnitude);

        std::string result;
        int count = 0;
        for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
            if (count > 0 && count % 3 == 0) result += ',';
            result += *it;
            ++count;
        }
        if (negative) result += '-';

        std::cout << std::string(result.rbegin(), result.rend()) << '\n';
    }
    return 0;
}
