#include <iostream>
#include <string>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::cin.ignore();

    std::vector<std::string> lines(static_cast<std::size_t>(n));
    for (std::string& line : lines) std::getline(std::cin, line);

    // TODO: 没有边框，也没有按最长行补齐。
    std::cout << "+--+\n";
    for (const std::string& line : lines) std::cout << line << '\n';
    std::cout << "+--+\n";
    return 0;
}
