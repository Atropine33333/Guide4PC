// 参考实现：text-line-stats
#include <iostream>
#include <string>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    std::string line;
    std::getline(std::cin, line);

    long long words = 0;
    long long chars = 0;
    for (int i = 0; i < n; ++i) {
        std::getline(std::cin, line);
        chars += static_cast<long long>(line.size());
        bool inWord = false;
        for (char ch : line) {
            if (ch == ' ') {
                inWord = false;
            } else if (!inWord) {
                inWord = true;
                ++words;
            }
        }
    }

    std::cout << n << ' ' << words << ' ' << chars << '\n';
    return 0;
}
