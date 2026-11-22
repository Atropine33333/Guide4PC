#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        // TODO: 从右往左每三位插入逗号，负数保留前导负号。
        std::cout << value << '\n';
    }
    return 0;
}
