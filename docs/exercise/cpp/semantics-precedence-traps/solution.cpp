// Reference solution: semantics-precedence-traps
#include <iostream>

int main() {
    int q = 0;
    int a = 0;
    int b = 0;
    int c = 0;
    if (!(std::cin >> q >> a >> b >> c)) return 0;

    switch (q) {
        case 1: std::cout << a + b * c; break;
        case 2: std::cout << (a + b) * c; break;
        case 3: std::cout << (a << (b - 1)); break;
        case 4: std::cout << ((a & b) ^ c); break;
        case 5: std::cout << ((a < b) == (c > a)); break;
        case 6: std::cout << (-a * -b); break;
        case 7: std::cout << (a - b - c); break;
        case 8: std::cout << (a ? b : c); break;
        default: std::cout << 0; break;
    }
    std::cout << '\n';
    return 0;
}
