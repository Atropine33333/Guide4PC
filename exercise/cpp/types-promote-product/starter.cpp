#include <iostream>

int main() {
    int a = 0;
    int b = 0;
    int c = 0;
    std::cin >> a >> b >> c;

    // TODO: 先用 64 位整数提升，再相乘。
    const int product = a * b * c;

    std::cout << product << '\n';
    return 0;
}
