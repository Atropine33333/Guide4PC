// Reference solution: debugging-leap-year
#include <iostream>

bool is_leap(int year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

int main() {
    int year = 0;
    std::cin >> year;
    std::cout << (is_leap(year) ? "leap" : "common") << '\n';
    return 0;
}
