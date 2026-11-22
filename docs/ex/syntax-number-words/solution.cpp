// Reference solution: syntax-number-words
#include <iostream>
#include <string>

namespace {

const char* const kSmall[] = {
    "zero", "one", "two", "three", "four", "five", "six", "seven", "eight",
    "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen"};

const char* const kTens[] = {"",      "",      "twenty",  "thirty", "forty",
                             "fifty", "sixty", "seventy", "eighty", "ninety"};

std::string below_hundred(int value) {
    if (value < 20) return kSmall[value];
    std::string result = kTens[value / 10];
    if (value % 10 != 0) {
        result += ' ';
        result += kSmall[value % 10];
    }
    return result;
}

std::string below_thousand(int value) {
    if (value < 100) return below_hundred(value);
    std::string result = std::string(kSmall[value / 100]) + " hundred";
    if (value % 100 != 0) {
        result += ' ';
        result += below_hundred(value % 100);
    }
    return result;
}

}  // namespace

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    if (n < 1000) {
        std::cout << below_thousand(n) << '\n';
        return 0;
    }

    std::cout << below_thousand(n / 1000) << " thousand";
    if (n % 1000 != 0) std::cout << ' ' << below_thousand(n % 1000);
    std::cout << '\n';
    return 0;
}
