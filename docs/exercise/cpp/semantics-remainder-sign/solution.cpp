// 参考实现：semantics-remainder-sign
#include <iostream>

int main() {
    long long a = 0;
    long long b = 0;
    std::cin >> a >> b;

    const long long remainder = a % b;
    const long long modulus = b < 0 ? -b : b;
    const long long normalized = remainder < 0 ? remainder + modulus : remainder;

    std::cout << remainder << ' ' << normalized << '\n';
    return 0;
}
