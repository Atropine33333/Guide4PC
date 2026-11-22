#include <iostream>

int main() {
    int percent = 0;
    int width = 0;
    std::cin >> percent >> width;

    // TODO: 计算已完成的格子数，输出 # 与 - 组成的进度条。
    std::cout << '[';
    for (int i = 0; i < width; ++i) {
        std::cout << '-';
    }
    std::cout << "] " << percent << "%\n";
    return 0;
}
