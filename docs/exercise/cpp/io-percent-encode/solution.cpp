// 参考实现：io-percent-encode
#include <iostream>
#include <string>

namespace {

bool is_safe(unsigned char c) {
    if (c >= 'A' && c <= 'Z') return true;
    if (c >= 'a' && c <= 'z') return true;
    if (c >= '0' && c <= '9') return true;
    return c == '-' || c == '_' || c == '.' || c == '~';
}

}  // namespace

int main() {
    std::string line;
    if (!std::getline(std::cin, line)) return 0;

    static const char hex[] = "0123456789ABCDEF";
    for (char ch : line) {
        const unsigned char c = static_cast<unsigned char>(ch);
        if (is_safe(c)) {
            std::cout << c;
        } else {
            std::cout << '%' << hex[(c >> 4) & 0x0F] << hex[c & 0x0F];
        }
    }
    std::cout << '\n';
    return 0;
}
