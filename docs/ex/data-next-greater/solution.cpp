// 参考实现：data-next-greater
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<long long> values(static_cast<std::size_t>(n));
    for (long long& value : values) std::cin >> value;

    std::vector<int> answer(static_cast<std::size_t>(n), -1);
    std::vector<int> stack;
    stack.reserve(static_cast<std::size_t>(n));
    for (int i = n - 1; i >= 0; --i) {
        const long long current = values[static_cast<std::size_t>(i)];
        while (!stack.empty() &&
               values[static_cast<std::size_t>(stack.back())] <= current) {
            stack.pop_back();
        }
        if (!stack.empty()) answer[static_cast<std::size_t>(i)] = stack.back();
        stack.push_back(i);
    }

    for (int i = 0; i < n; ++i) {
        if (i > 0) std::cout << ' ';
        const int index = answer[static_cast<std::size_t>(i)];
        if (index < 0) {
            std::cout << -1;
        } else {
            std::cout << values[static_cast<std::size_t>(index)];
        }
    }
    std::cout << '\n';
    return 0;
}
