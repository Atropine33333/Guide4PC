// 参考实现：stdlib-replace-all
#include <iostream>
#include <string>

int main() {
    std::string text, pattern, replacement;
    std::getline(std::cin, text);
    std::getline(std::cin, pattern);
    std::getline(std::cin, replacement);

    std::string result;
    std::size_t pos = 0;
    while (true) {
        const std::size_t found = text.find(pattern, pos);
        if (found == std::string::npos) {
            result.append(text, pos, std::string::npos);
            break;
        }
        result.append(text, pos, found - pos);
        result += replacement;
        pos = found + pattern.size();
    }

    std::cout << result << '\n';
    return 0;
}
