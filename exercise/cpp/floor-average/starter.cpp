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

    // TODO: 先算截断平均，再根据余数的符号修正出向下平均。
    long long truncating = total / n;
    long long floorAverage = truncating;

    std::cout << truncating << ' ' << floorAverage << '\n';
    return 0;
}
