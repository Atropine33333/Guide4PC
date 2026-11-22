// 参考实现：stdlib-next-permutation
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) std::cin >> values[i];

    std::next_permutation(values.begin(), values.end());

    for (int i = 0; i < n; ++i) {
        if (i != 0) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
    return 0;
}
