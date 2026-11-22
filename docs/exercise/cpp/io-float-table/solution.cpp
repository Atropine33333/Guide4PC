// 参考实现：io-float-table
#include <iomanip>
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < n; ++i) {
        double value = 0.0;
        std::cin >> value;
        std::cout << std::setw(10) << std::right << value << '\n';
    }
    return 0;
}
