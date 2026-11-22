#include <iostream>
#include <string>
#include <vector>

int main() {
    int m = 0;
    std::cin >> m;

    std::vector<int> values;
    for (int i = 0; i < m; ++i) {
        std::string op;
        std::cin >> op;
        if (op == "push_back") {
            int x = 0;
            std::cin >> x;
            values.push_back(x);
        } else if (op == "push_front") {
            int x = 0;
            std::cin >> x;
            // TODO: vector 不能高效地在头部插入。
            (void)x;
        } else if (op == "pop_back") {
            if (values.empty()) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values.back() << '\n';
                values.pop_back();
            }
        } else if (op == "pop_front") {
            // TODO: 弹出队头。
            std::cout << "EMPTY\n";
        } else if (op == "size") {
            std::cout << values.size() << '\n';
        }
    }
    return 0;
}
