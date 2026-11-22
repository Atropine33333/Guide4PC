// 参考实现：debugging-run-length
#include <iostream>
#include <string>

int main() {
    std::string text;
    std::cin >> text;

    char current = text[0];
    int count = 1;
    for (std::size_t i = 1; i < text.size(); ++i) {
        if (text[i] == current) {
            ++count;
        } else {
            std::cout << current << count;
            current = text[i];
            count = 1;
        }
    }
    std::cout << current << count << '\n';
    return 0;
}
