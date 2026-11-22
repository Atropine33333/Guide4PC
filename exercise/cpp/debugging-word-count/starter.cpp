#include <iostream>
#include <string>

int main() {
    int lines = 0;
    int words = 0;
    std::string line;
    while (std::getline(std::cin, line)) {
        ++lines;
        words += 1;
        for (char ch : line) {
            if (ch == ' ') ++words;
        }
    }
    std::cout << lines << ' ' << words << '\n';
    return 0;
}
