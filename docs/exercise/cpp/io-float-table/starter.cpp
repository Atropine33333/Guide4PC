#include <iomanip>
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        double value = 0.0;
        std::cin >> value;
        // TODO: 两位小数、右对齐、整行宽度至少 10。
        std::cout << value << '\n';
    }
    return 0;
}
