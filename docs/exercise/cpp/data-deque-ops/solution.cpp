// 参考实现：data-deque-ops
#include <deque>
#include <iostream>
#include <string>

int main() {
    int m = 0;
    std::cin >> m;

    std::deque<int> values;
    for (int i = 0; i < m; ++i) {
        std::string op;
        std::cin >> op;
        if (op == "push_front") {
            int x = 0;
            std::cin >> x;
            values.push_front(x);
        } else if (op == "push_back") {
            int x = 0;
            std::cin >> x;
            values.push_back(x);
        } else if (op == "pop_front") {
            if (values.empty()) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values.front() << '\n';
                values.pop_front();
            }
        } else if (op == "pop_back") {
            if (values.empty()) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values.back() << '\n';
                values.pop_back();
            }
        } else if (op == "size") {
            std::cout << values.size() << '\n';
        }
    }
    return 0;
}
