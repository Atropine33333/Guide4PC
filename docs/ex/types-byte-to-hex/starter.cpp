#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        // TODO: 输出两位大写十六进制，不足两位补 0。
        std::cout << value << '\n';
    }
    return 0;
}
