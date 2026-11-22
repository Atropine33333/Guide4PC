#include <iostream>
#include <string>

int main() {
    long long k = 0;
    std::cin >> k;
    std::cin.ignore();

    std::string line;
    std::getline(std::cin, line);

    for (char& c : line) {
        if (c >= 'a' && c <= 'z') {
            // TODO: 处理负位移与回绕；大写字母也还没有处理。
            c = static_cast<char>(c + k % 26);
        }
    }

    std::cout << line << '\n';
    return 0;
}
