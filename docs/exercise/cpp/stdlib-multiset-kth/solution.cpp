#include <iostream>
#include <iterator>
#include <set>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q = 0;
    if (!(std::cin >> q)) {
        return 0;
    }
    std::multiset<int> values;

    for (int i = 0; i < q; ++i) {
        std::string op;
        std::cin >> op;
        if (op == "add") {
            int x = 0;
            std::cin >> x;
            values.insert(x);
        } else if (op == "erase") {
            int x = 0;
            std::cin >> x;
            const auto it = values.find(x);
            if (it != values.end()) {
                values.erase(it);
            }
        } else {
            int k = 0;
            std::cin >> k;
            if (k < 1 || k > static_cast<int>(values.size())) {
                std::cout << "none\n";
            } else {
                std::cout << *std::next(values.begin(), k - 1) << '\n';
            }
        }
    }
    return 0;
}
