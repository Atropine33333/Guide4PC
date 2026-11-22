// 参考实现：swap-sequence
#include <iostream>
#include <vector>

// 参数必须是引用，函数才能改到调用者的元素。
void swapValues(int& a, int& b) {
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
