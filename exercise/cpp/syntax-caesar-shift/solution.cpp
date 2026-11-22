// 参考实现：syntax-caesar-shift
#include <iostream>
#include <string>

int main() {
    long long k = 0;
    std::cin >> k;
    std::cin.ignore();

    std::string line;
    std::getline(std::cin, line);

    const long long shift = ((k % 26) + 26) % 26;
    for (char& c : line) {
        if (c >= 'a' && c <= 'z') {
            c = static_cast<char>('a' + (c - 'a' + shift) % 26);
        } else if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>('A' + (c - 'A' + shift) % 26);
        }
    }

    std::cout << line << '\n';
    return 0;
}
