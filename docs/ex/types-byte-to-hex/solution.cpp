// 参考实现：types-byte-to-hex
#include <iostream>

int main() {
    static const char kDigits[] = "0123456789ABCDEF";

    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        const unsigned char byte = static_cast<unsigned char>(value);
        std::cout << kDigits[(byte >> 4) & 0x0F] << kDigits[byte & 0x0F] << '\n';
    }
    return 0;
}
