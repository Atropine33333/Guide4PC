#include <iostream>

int main() {
    long long a = 0;
    long long b = 0;
    std::cin >> a >> b;

    const long long remainder = a % b;
    // TODO: 把 remainder 归一化到 [0, |b|)。b 为负数时也要正确。
    const long long normalized = remainder;

    std::cout << remainder << ' ' << normalized << '\n';
    return 0;
}
