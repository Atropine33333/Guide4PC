#include <iostream>
#include <string>

int main() {
    char mode = 'E';
    if (!(std::cin >> mode)) {
        return 0;
    }

    const char* kHex = "0123456789ABCDEF";
    if (mode == 'E') {
        std::string digits;
        std::cin >> digits;
        if (digits.size() % 2 != 0) {
            digits.insert(digits.begin(), '0');
        }
        for (std::size_t i = 0; i + 1 < digits.size(); i += 2) {
            if (i != 0) {
                std::cout << ' ';
            }
            std::cout << kHex[digits[i] - '0'] << kHex[digits[i + 1] - '0'];
        }
        std::cout << '\n';
    } else {
        std::string token;
        std::string digits;
        while (std::cin >> token) {
            digits += token;
        }
        const std::size_t start = digits.find_first_not_of('0');
        if (start == std::string::npos) {
            std::cout << "0\n";
        } else {
            std::cout << digits.substr(start) << '\n';
        }
    }
    return 0;
}
