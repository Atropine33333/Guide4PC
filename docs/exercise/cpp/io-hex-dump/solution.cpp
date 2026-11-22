// 参考实现：io-hex-dump
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

void print_hex2(unsigned char byte) {
    static const char digits[] = "0123456789abcdef";
    std::cout << digits[(byte >> 4) & 0x0F] << digits[byte & 0x0F];
}

void print_offset(std::size_t offset) {
    char digits[8];
    for (int i = 7; i >= 0; --i) {
        digits[i] = "0123456789abcdef"[offset & 0x0F];
        offset >>= 4;
    }
    for (int i = 0; i < 8; ++i) std::cout << digits[i];
}

}  // namespace

int main() {
    std::vector<unsigned char> bytes;
    char ch = 0;
    while (std::cin.get(ch)) bytes.push_back(static_cast<unsigned char>(ch));

    for (std::size_t start = 0; start < bytes.size(); start += 16) {
        const std::size_t count = bytes.size() - start < 16 ? bytes.size() - start : 16;
        print_offset(start);
        std::cout << "  ";
        for (std::size_t i = 0; i < 16; ++i) {
            if (i < count) {
                print_hex2(bytes[start + i]);
                std::cout << ' ';
            } else {
                std::cout << "   ";
            }
        }
        std::cout << '|';
        for (std::size_t i = 0; i < count; ++i) {
            const unsigned char byte = bytes[start + i];
            std::cout << (byte >= 32 && byte <= 126 ? static_cast<char>(byte) : '.');
        }
        std::cout << "|\n";
    }
    return 0;
}
