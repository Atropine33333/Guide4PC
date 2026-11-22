#include <iostream>

int main() {
    int q = 0;
    int a = 0;
    int b = 0;
    int c = 0;
    std::cin >> q >> a >> b >> c;

    if (q == 1) {
        std::cout << a + b * c << '\n';
    } else if (q == 2) {
        std::cout << (a + b) * c << '\n';
    } else {
        // TODO: handle expressions 3 through 8.
        std::cout << 0 << '\n';
    }
    return 0;
}
