#include <cstdint>
#include <iomanip>
#include <iostream>

int main() {
    std::uint32_t x = 0;
    std::cin >> x;

    // TODO: extract the four bytes from low to high.
    std::cout << x << " 0 0 0\n";

    // TODO: reverse the byte order before printing.
    std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << x << '\n';
    return 0;
}
