// 参考实现：floor-average
#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    long long total = 0;
    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        total += value;
    }

    const long long truncating = total / n;
    long long floorAverage = truncating;
    if (total % n != 0 && total < 0) {
        --floorAverage;
    }

    std::cout << truncating << ' ' << floorAverage << '\n';
    return 0;
}
