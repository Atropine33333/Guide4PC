#include <iostream>
#include <vector>

// 交换 a 与 b 指向的两个元素。
void swapValues(int a, int b) {
    int temporary = a;
    a = b;
    b = temporary;
}

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    std::vector<int> values(n);
    for (int& value : values) {
        std::cin >> value;
    }

    int m = 0;
    std::cin >> m;
    for (int k = 0; k < m; ++k) {
        int i = 0;
        int j = 0;
        std::cin >> i >> j;
        swapValues(values[i - 1], values[j - 1]);
    }

    for (int i = 0; i < n; ++i) {
        if (i != 0) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
    return 0;
}
