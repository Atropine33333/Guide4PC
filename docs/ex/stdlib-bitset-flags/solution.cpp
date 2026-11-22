// Reference solution: stdlib-bitset-flags
#include <bitset>
#include <cstddef>
#include <iostream>
#include <string>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    std::bitset<64> flags;
    for (int k = 0; k < n; ++k) {
        std::string command;
        if (!(std::cin >> command)) return 0;

        if (command == "set" || command == "reset" || command == "flip" ||
            command == "test") {
            int i = 0;
            std::cin >> i;
            const std::size_t index = static_cast<std::size_t>(i);
            if (command == "set") {
                flags.set(index);
            } else if (command == "reset") {
                flags.reset(index);
            } else if (command == "flip") {
                flags.flip(index);
            } else {
                std::cout << flags.test(index) << '\n';
            }
        } else if (command == "count") {
            std::cout << flags.count() << '\n';
        } else if (command == "first") {
            int found = -1;
            for (int i = 0; i < 64; ++i) {
                if (flags.test(static_cast<std::size_t>(i))) {
                    found = i;
                    break;
                }
            }
            std::cout << found << '\n';
        } else if (command == "last") {
            int found = -1;
            for (int i = 63; i >= 0; --i) {
                if (flags.test(static_cast<std::size_t>(i))) {
                    found = i;
                    break;
                }
            }
            std::cout << found << '\n';
        } else if (command == "any") {
            std::cout << (flags.any() ? 1 : 0) << '\n';
        } else if (command == "fill") {
            flags.set();
        }
    }
    return 0;
}
