#include <bitset>
#include <iostream>
#include <string>

int main() {
    int n = 0;
    std::cin >> n;

    std::bitset<64> flags;
    for (int k = 0; k < n; ++k) {
        std::string command;
        std::cin >> command;

        if (command == "set") {
            int i = 0;
            std::cin >> i;
            flags.set(static_cast<std::size_t>(i));
        } else if (command == "test") {
            int i = 0;
            std::cin >> i;
            std::cout << flags.test(static_cast<std::size_t>(i)) << '\n';
        } else {
            // TODO: reset, flip, count, first, last, any, fill.
            std::cout << "TODO\n";
        }
    }
    return 0;
}
