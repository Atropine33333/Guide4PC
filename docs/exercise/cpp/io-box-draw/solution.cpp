// 参考实现：io-box-draw
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::cin.ignore();

    std::vector<std::string> lines(static_cast<std::size_t>(n));
    std::size_t widest = 0;
    for (std::string& line : lines) {
        std::getline(std::cin, line);
        widest = std::max(widest, line.size());
    }

    const std::string border(widest + 2, '-');
    std::cout << '+' << border << "+\n";
    for (const std::string& line : lines) {
        std::cout << "| " << line << std::string(widest - line.size(), ' ')
                  << " |\n";
    }
    std::cout << '+' << border << "+\n";
    return 0;
}
