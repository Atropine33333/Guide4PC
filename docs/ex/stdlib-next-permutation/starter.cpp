#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) std::cin >> values[i];

    // TODO: 求出下一个字典序排列；最后一个排列要回到升序。
    for (int i = 0; i < n; ++i) {
        if (i != 0) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
    return 0;
}
