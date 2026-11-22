// 参考实现：io-column-report
#include <iomanip>
#include <iostream>
#include <string>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        std::string name;
        long long score = 0;
        std::cin >> name >> score;

        std::cout << std::left << std::setw(12) << name << '|'
                  << std::right << std::setw(8) << score << '\n';
    }
    return 0;
}
