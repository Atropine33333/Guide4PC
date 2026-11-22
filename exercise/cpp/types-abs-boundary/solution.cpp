#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        unsigned long long magnitude = 0;
        if (value < 0) {
            magnitude = static_cast<unsigned long long>(-(value + 1)) + 1ULL;
        } else {
            magnitude = static_cast<unsigned long long>(value);
        }
        std::cout << magnitude << '\n';
    }
    return 0;
}
