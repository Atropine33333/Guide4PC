// 参考实现：syntax-triangle-kind
#include <iostream>

int main() {
    long long a = 0, b = 0, c = 0;
    std::cin >> a >> b >> c;

    if (a + b <= c || a + c <= b || b + c <= a) {
        std::cout << "NOT_TRIANGLE\n";
    } else if (a == b && b == c) {
        std::cout << "EQUILATERAL\n";
    } else if (a == b || b == c || a == c) {
        std::cout << "ISOSCELES\n";
    } else {
        std::cout << "SCALENE\n";
    }
    return 0;
}
