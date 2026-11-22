// 参考实现：syntax-palindrome-line
#include <cctype>
#include <string>

bool is_palindrome(const std::string& text) {
    std::size_t left = 0;
    std::size_t right = text.size();

    while (left < right) {
        while (left < right &&
               !std::isalnum(static_cast<unsigned char>(text[left]))) {
            ++left;
        }
        while (left < right &&
               !std::isalnum(static_cast<unsigned char>(text[right - 1]))) {
            --right;
        }
        if (left >= right) break;

        const unsigned char a = static_cast<unsigned char>(text[left]);
        const unsigned char b = static_cast<unsigned char>(text[right - 1]);
        if (std::tolower(a) != std::tolower(b)) return false;
        ++left;
        --right;
    }
    return true;
}
