// 参考实现：debugging-word-count
#include <cctype>
#include <iostream>
#include <string>

int main() {
    int lines = 0;
    int words = 0;
    std::string line;
    while (std::getline(std::cin, line)) {
        bool has_text = false;
        bool in_word = false;
        for (char ch : line) {
            if (std::isspace(static_cast<unsigned char>(ch))) {
                in_word = false;
            } else {
                has_text = true;
                if (!in_word) {
                    ++words;
                    in_word = true;
                }
            }
        }
        if (has_text) ++lines;
    }
    std::cout << lines << ' ' << words << '\n';
    return 0;
}
