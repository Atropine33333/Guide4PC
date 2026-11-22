#include <iostream>
#include <string>

int main() {
    int width = 0;
    std::cin >> width;
    std::cin.ignore();

    std::string text;
    std::getline(std::cin, text);

    // TODO: 按 width 贪心折行，而不是原样输出。
    std::cout << text << '\n';
    return 0;
}
