#include <iostream>
#include <string>

int main() {
    std::string line;
    int number = 0;
    while (std::getline(std::cin, line)) {
        ++number;
        // TODO: the width depends on the total number of lines.
        std::cout << number << '|' << line << '\n';
    }
    return 0;
}
