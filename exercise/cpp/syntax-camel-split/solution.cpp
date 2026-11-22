#include <cctype>
#include <iostream>
#include <string>

int main() {
    std::string text;
    std::cin >> text;

    std::string out;
    std::string word;
    for (std::size_t i = 0; i < text.size(); ++i) {
        const unsigned char current = static_cast<unsigned char>(text[i]);
        if (i > 0 && std::isupper(current)) {
            const unsigned char prev = static_cast<unsigned char>(text[i - 1]);
            const bool after_word = std::islower(prev) || std::isdigit(prev);
            const bool acronym_break =
                std::isupper(prev) && i + 1 < text.size() &&
                std::islower(static_cast<unsigned char>(text[i + 1]));
            if (after_word || acronym_break) {
                if (!out.empty()) out += ' ';
                out += word;
                word.clear();
            }
        }
        word += static_cast<char>(std::tolower(current));
    }
    if (!out.empty()) out += ' ';
    out += word;

    std::cout << out << '\n';
    return 0;
}
