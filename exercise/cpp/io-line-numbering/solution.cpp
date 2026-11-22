// Reference solution: io-line-numbering
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(std::cin, line)) lines.push_back(line);

    int width = 1;
    for (std::size_t count = lines.size(); count >= 10; count /= 10) ++width;

    for (std::size_t i = 0; i < lines.size(); ++i) {
        std::cout << std::setw(width) << static_cast<int>(i) + 1 << '|'
                  << lines[i] << '\n';
    }
    return 0;
}
