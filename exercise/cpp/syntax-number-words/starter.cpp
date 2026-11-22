#include <iostream>
#include <string>

int main() {
    int n = 0;
    std::cin >> n;

    const char* small[] = {"zero", "one", "two", "three", "four", "five",
                           "six", "seven", "eight", "nine", "ten", "eleven",
                           "twelve", "thirteen", "fourteen", "fifteen",
                           "sixteen", "seventeen", "eighteen", "nineteen"};

    if (n < 20) {
        std::cout << small[n] << '\n';
    } else {
        // TODO: handle tens, hundreds and thousands.
        std::cout << n << '\n';
    }
    return 0;
}
