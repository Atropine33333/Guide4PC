#include <iostream>

int main() {
    int a = 0, b = 0;
    std::cin >> a >> b;

    std::cout << std::boolalpha;
    std::cout << (a < b) << '\n';
    std::cout << (a == b) << '\n';

    // TODO: 后三行要用短路保护除法和取模。
    std::cout << false << '\n';
    std::cout << false << '\n';
    std::cout << false << '\n';
    return 0;
}
