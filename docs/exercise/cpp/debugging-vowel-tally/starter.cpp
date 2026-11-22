#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int vowels = 0;
    for (int i = 0; i < n - 1; ++i) {  // BUG: 少检查了最后一个字符？
        char c = 0;
        std::cin >> c;
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            ++vowels;
        }
    }

    std::cout << vowels << '\n';
    return 0;
}
