#include <cmath>
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        double value = 0.0;
        std::cin >> value;

        // TODO: this rounds ties away from zero; use round-half-to-even.
        const long long rounded = std::llround(value);
        std::cout << rounded << '\n';
    }
    return 0;
}
