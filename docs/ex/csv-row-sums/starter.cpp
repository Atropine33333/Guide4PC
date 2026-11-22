#include <iostream>
#include <string>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    std::string line;
    std::getline(std::cin, line);  // 读掉第一行剩下的换行符

    for (int row = 0; row < n; ++row) {
        std::getline(std::cin, line);
        long long sum = 0;
        // TODO: 逐个字符解析 line，用逗号分隔字段；空字段按 0 计入。
        std::cout << sum << '\n';
    }
    return 0;
}
