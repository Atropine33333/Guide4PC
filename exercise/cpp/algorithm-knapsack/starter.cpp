#include <iostream>
#include <vector>

int main() {
    int n = 0;
    int capacity = 0;
    std::cin >> n >> capacity;

    std::vector<long long> best(static_cast<std::size_t>(capacity) + 1, 0);
    for (int i = 0; i < n; ++i) {
        int weight = 0;
        long long value = 0;
        std::cin >> weight >> value;

        // TODO: process the item in the correct direction and skip it when too heavy.
        if (weight <= capacity) {
            best[static_cast<std::size_t>(capacity)] += value;
        }
    }

    std::cout << best[static_cast<std::size_t>(capacity)] << '\n';
    return 0;
}
