#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

int main() {
    int m = 0;
    int target = 0;
    std::cin >> m >> target;
    std::vector<int> coins(m);
    for (int& coin : coins) {
        std::cin >> coin;
    }

    const int kInf = std::numeric_limits<int>::max() / 2;
    std::vector<int> best(static_cast<std::size_t>(target) + 1, kInf);
    best[0] = 0;
    for (int amount = 1; amount <= target; ++amount) {
        for (int coin : coins) {
            if (coin <= amount && best[amount - coin] + 1 < best[amount]) {
                best[amount] = best[amount - coin] + 1;
            }
        }
    }

    if (best[target] >= kInf) {
        std::cout << -1 << '\n';
    } else {
        std::cout << best[target] << '\n';
    }
    return 0;
}
