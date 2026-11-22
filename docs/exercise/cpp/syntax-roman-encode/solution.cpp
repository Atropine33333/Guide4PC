// 参考实现：syntax-roman-encode
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *symbols[] = {"M", "CM", "D", "CD", "C", "XC",
                             "L", "XL", "X", "IX", "V", "IV", "I"};

    for (int i = 0; i < 13; ++i) {
        while (n >= values[i]) {
            std::cout << symbols[i];
            n -= values[i];
        }
    }
    std::cout << '\n';
    return 0;
}
