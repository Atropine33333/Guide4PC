#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    // TODO: 读入 n 个数，边读边更新最小值、最大值与总和。
    int minimum = 0;
    int maximum = 0;
    long long total = 0;

    std::cout << minimum << ' ' << maximum << ' ' << total << '\n';
    return 0;
}
