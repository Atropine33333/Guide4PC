#include <cmath>
#include <iostream>

int main() {
    long long a = 0, b = 0, m = 0;
    std::cin >> a >> b >> m;

    // TODO: 用浮点幂再取模，指数大时结果完全不对。
    const long long value = static_cast<long long>(std::pow(a, b));
    std::cout << value % m << '\n';
    return 0;
}
