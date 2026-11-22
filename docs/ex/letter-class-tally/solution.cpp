// 参考实现：letter-class-tally
#include <iostream>
#include <string>

int main() {
    std::string word;
    if (!(std::cin >> word)) return 0;

    int uppercase = 0;
    int lowercase = 0;
    int digits = 0;

    for (char c : word) {
        if (c >= 'A' && c <= 'Z') {
            ++uppercase;
        } else if (c >= 'a' && c <= 'z') {
            ++lowercase;
        } else if (c >= '0' && c <= '9') {
            ++digits;
        }
    }

    std::cout << uppercase << ' ' << lowercase << ' ' << digits << '\n';
    return 0;
}
