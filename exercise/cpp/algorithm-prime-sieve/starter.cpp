#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int count = 0;
    int largest = 0;
    for (int i = 2; i <= n; ++i) {
        // TODO: 这不是素数判定；请换成埃氏筛或正确的判定方法。
        if (i % 2 == 1) {
            ++count;
            largest = i;
        }
    }

    std::cout << count << ' ' << largest << '\n';
    return 0;
}
