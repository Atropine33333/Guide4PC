// 参考实现：debugging-vowel-tally
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int vowels = 0;
    for (int i = 0; i < n; ++i) {
        char c = 0;
        std::cin >> c;
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            ++vowels;
        }
    }

    std::cout << vowels << '\n';
    return 0;
}
