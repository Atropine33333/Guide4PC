#include <cstdint>
#include <iostream>
#include <vector>

int main() {
    std::vector<unsigned char> bytes;
    char ch = 0;
    while (std::cin.get(ch)) bytes.push_back(static_cast<unsigned char>(ch));

    // TODO: 每 16 字节一行，输出偏移、十六进制区和 ASCII 区。
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        std::cout << static_cast<int>(bytes[i]) << ' ';
    }
    std::cout << '\n';
    return 0;
}
