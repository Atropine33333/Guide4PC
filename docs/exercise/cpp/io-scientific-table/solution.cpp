#include <iomanip>
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        double value = 0.0;
        std::cin >> value;
        std::cout << std::scientific << std::setprecision(3) << std::right
                  << std::setw(12) << value << '\n';
    }
    return 0;
}
