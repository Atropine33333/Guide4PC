#include <iostream>

bool is_leap(int year) {
    // BUG: this misses the century rules.
    return year % 4 == 0;
}

int main() {
    int year = 0;
    std::cin >> year;
    std::cout << (is_leap(year) ? "leap" : "common") << '\n';
    return 0;
}
