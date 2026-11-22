#include <iostream>
#include <string>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    long long words = 0;
    long long chars = 0;
    std::string token;
    while (std::cin >> token) {
        ++words;
        chars += static_cast<long long>(token.size());
    }

    // TODO: 上面的循环会跳过空行和空格，统计结果不对。
    // 改成先读掉第一行行尾的换行符，再逐行 getline 统计。
    std::cout << n << ' ' << words << ' ' << chars << '\n';
    return 0;
}
