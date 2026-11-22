// Reference solution: algorithm-knapsack
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    int capacity = 0;
    if (!(std::cin >> n >> capacity)) return 0;

    std::vector<long long> best(static_cast<std::size_t>(capacity) + 1, 0);
    for (int i = 0; i < n; ++i) {
        int weight = 0;
        long long value = 0;
        std::cin >> weight >> value;
        if (weight > capacity) continue;
        for (int c = capacity; c >= weight; --c) {
            const std::size_t current = static_cast<std::size_t>(c);
            const std::size_t previous = static_cast<std::size_t>(c - weight);
            best[current] = std::max(best[current], best[previous] + value);
        }
    }

    std::cout << best[static_cast<std::size_t>(capacity)] << '\n';
    return 0;
}
