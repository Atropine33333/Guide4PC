#include <iostream>

int main() {
    unsigned long long x = 0;
    if (!(std::cin >> x)) return 0;

    int setBits = 0;
    unsigned long long highest = 0;
    long long signedValue = 0;

    // TODO: 逐位统计置位数与最高置位，并计算有符号解释值。

    std::cout << setBits << ' ' << highest << ' ' << signedValue << '\n';
    return 0;
}
