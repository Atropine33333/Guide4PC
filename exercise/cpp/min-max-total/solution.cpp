// 参考实现：min-max-total
#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    long long first = 0;
    if (!(std::cin >> first)) return 0;

    long long minimum = first;
    long long maximum = first;
    long long total = first;

    for (int i = 1; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        if (value < minimum) minimum = value;
        if (value > maximum) maximum = value;
        total += value;
    }

    std::cout << minimum << ' ' << maximum << ' ' << total << '\n';
    return 0;
}
