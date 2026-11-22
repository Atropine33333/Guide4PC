#include <iomanip>
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        double value = 0.0;
        std::cin >> value;
        // TODO: 科学计数法、三位小数、整行宽度 12。
        std::cout << value << '\n';
    }
    return 0;
}
