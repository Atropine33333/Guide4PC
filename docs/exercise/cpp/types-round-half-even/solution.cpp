// Reference solution: types-round-half-even
#include <cmath>
#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    for (int i = 0; i < n; ++i) {
        double value = 0.0;
        if (!(std::cin >> value)) return 0;

        const double nearest = std::nearbyint(value);
        const long long rounded = static_cast<long long>(nearest);
        if (rounded == 0) {
            std::cout << 0 << '\n';
        } else {
            std::cout << rounded << '\n';
        }
    }
    return 0;
}
