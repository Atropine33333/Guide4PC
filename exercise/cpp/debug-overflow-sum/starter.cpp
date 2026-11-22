#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    int total = 0;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        total += value;
    }

    // TODO: 这段代码能编译，但在部分测试数据上答案错误。找出原因并修复。
    std::cout << total << '\n';
    return 0;
}
