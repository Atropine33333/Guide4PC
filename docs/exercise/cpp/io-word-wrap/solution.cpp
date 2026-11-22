// 参考实现：io-word-wrap
#include <iostream>
#include <string>
#include <vector>

int main() {
    int width = 0;
    std::cin >> width;
    std::cin.ignore();

    std::string text;
    std::getline(std::cin, text);

    std::vector<std::string> words;
    std::string current;
    for (char c : text) {
        if (c == ' ') {
            words.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    words.push_back(current);

    std::string line;
    for (const std::string& word : words) {
        if (line.empty()) {
            line = word;
        } else if (static_cast<int>(line.size() + 1 + word.size()) <= width) {
            line += ' ';
            line += word;
        } else {
            std::cout << line << '\n';
            line = word;
        }
    }
    if (!line.empty()) std::cout << line << '\n';
    return 0;
}
