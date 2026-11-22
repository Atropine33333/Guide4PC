#include <iostream>
#include <string>

int main() {
    std::string text, pattern, replacement;
    std::getline(std::cin, text);
    std::getline(std::cin, pattern);
    std::getline(std::cin, replacement);

    // TODO: 只替换了第一处。
    const std::size_t pos = text.find(pattern);
    if (pos != std::string::npos) {
        text.replace(pos, pattern.size(), replacement);
    }
    std::cout << text << '\n';
    return 0;
}
