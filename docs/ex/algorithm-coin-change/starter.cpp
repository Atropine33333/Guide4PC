#include <iostream>
#include <vector>

int main() {
    int m = 0;
    int target = 0;
    std::cin >> m >> target;
    std::vector<int> coins(m);
    for (int& coin : coins) {
        std::cin >> coin;
    }

    // TODO: 用动态规划求最少硬币数；无法凑出时输出 -1。
    (void)target;
    std::cout << -1 << '\n';
    return 0;
}
