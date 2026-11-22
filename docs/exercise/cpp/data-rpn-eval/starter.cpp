#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<long long> stack;
    std::string token;

    while (std::cin >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            // TODO: 弹出右操作数和左操作数，计算后压回结果。
        } else {
            stack.push_back(std::stoll(token));
        }
    }

    std::cout << (stack.empty() ? 0 : stack.back()) << '\n';
    return 0;
}
