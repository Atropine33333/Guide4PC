// 参考实现：csv-row-sums
#include <iostream>
#include <string>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    std::string line;
    std::getline(std::cin, line);

    for (int row = 0; row < n; ++row) {
        std::getline(std::cin, line);
        long long sum = 0;
        long long current = 0;
        for (std::size_t i = 0; i < line.size(); ++i) {
            const char ch = line[i];
            if (ch == ',') {
                sum += current;
                current = 0;
            } else if (ch >= '0' && ch <= '9') {
                current = current * 10 + (ch - '0');
            }
        }
        sum += current;
        std::cout << sum << '\n';
    }
    return 0;
}
