// 参考实现：io-eof-tally
#include <iostream>

int main() {
    long long count = 0;
    long long sum = 0;
    long long maximum = 0;
    bool first = true;

    long long value = 0;
    while (std::cin >> value) {
        ++count;
        sum += value;
        if (first || value > maximum) {
            maximum = value;
            first = false;
        }
    }

    std::cout << count << '\n' << sum << '\n' << maximum << '\n';
    return 0;
}
