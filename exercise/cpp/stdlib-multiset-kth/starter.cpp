#include <iostream>
#include <set>
#include <string>

int main() {
    int q = 0;
    std::cin >> q;
    std::multiset<int> values;

    for (int i = 0; i < q; ++i) {
        std::string op;
        std::cin >> op;
        if (op == "add") {
            int x = 0;
            std::cin >> x;
            // TODO: 插入 x。
            (void)x;
        } else if (op == "erase") {
            int x = 0;
            std::cin >> x;
            // TODO: 删除一个 x。
            (void)x;
        } else {
            int k = 0;
            std::cin >> k;
            // TODO: 输出第 k 小，或 none。
            (void)k;
            std::cout << "none\n";
        }
    }
    return 0;
}
