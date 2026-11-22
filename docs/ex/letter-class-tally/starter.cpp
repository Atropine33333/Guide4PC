#include <iostream>
#include <string>

int main() {
    std::string word;
    if (!(std::cin >> word)) return 0;

    int uppercase = 0;
    int lowercase = 0;
    int digits = 0;

    // TODO: 遍历 word，按字符范围分别累计三类字符。
    for (char c : word) {
        (void)c;
    }

    std::cout << uppercase << ' ' << lowercase << ' ' << digits << '\n';
    return 0;
}
