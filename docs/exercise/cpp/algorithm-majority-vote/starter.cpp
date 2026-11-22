#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<int> values(n);
    for (int& value : values) {
        std::cin >> value;
    }

    // TODO: 找出出现次数严格超过 n/2 的元素，否则输出 none。
    std::cout << "none\n";
    return 0;
}
